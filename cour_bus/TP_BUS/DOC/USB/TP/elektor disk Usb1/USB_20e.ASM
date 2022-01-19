;; USB_20.ASM
;*******************************************************************************
;
; *******************************************************************************
;
; Target:
;       Cypress 7C63000 8bit RISC microcontroller with 1.5Mbps USB serial interface 
;       Dallas 1623: High Resolution Temperature Measurement Sensor
; Overview
;  There are four main sub-systems: USB, Thermometer, LED, and Button. 
;   The system is started in the main() routine at reset. This routine 
;   initializes the USB variables, the IO ports, the Thermometer logic, 
;   and the data space. All USB communication occurs on an interrupt basis.
;  First, Main() loops waiting for a USB reset.
;  After receiving a USB reset, Main() enables the Endpoint 0 interrupt and 
;   loops waiting for Setups which ultimately will the result in the device 
;   being enumerated.
;  Once the device has been enumerated on the USB, the main loop waits 10ms, 
;   polls the thermometer, updates the LED, and initializes end point 1 if 
;   appropriate.
; USB
;  Endpoint 0 is used to support Control Transfers and vendor specific 
;   requests. End point 1 is also available for interrupt requests handling
;   small packets of data (good for mouse, joystick, keyboard, thermometer, etc.).
;   However, it is not used in this code.
;  Each control transfer interrupts the processor and the subsequent routines
;   services it.
; Thermometer
;  A simple 9-bit temperature value is read from the thermometer every 10ms. At
;   startup, the thermometer is initialized and placed into a continuous mode 
;   storing internally the current temperature. Thereafter, the temperature is 
;   read synchronously and returned into the USB end point one FIFO buffer.
; LED
;  The LED is controlled by P13. When P13 goes low, the LED is turned on.
;   The LED indicates the status of the USB connection. Once this device has
;   "logically" been enumerated and configured to run on the serial bus, the LED
;   is illuminated. The LED supports adjusting the brightness intensity by first
;   setting the new brightens value (default: FFh = High) and then setting the 
;   brightness update field.
; Button
;  A momentary push button is used to indicate that the application's 
;   Celsius/Fahrenheit display mode should be toggled.
;  With each GetTemperature request, a value is sent indicating whether the
;   button has been pushed.  
;  The GPIO interrupt is triggered by pushing the button causing its 
;   level to change from High to Low. A 100ms debounce was added to control the
;   erroneous re-occurrence of this logical state change for a period. The
;   1024ms timer decrements the debounce to zero, re-enabling the button if at the
;   end of the time out it has returned High.
;
; Port Usage
;  P0.0 - Thermometer Data  (input/output)
;    .1 - Thermometer Clock (output)
;    .2 - Thermometer Reset (output)
;    .3 - 
;    .4 -
;    .5 -
;    .6 -
;    .7 -
;  P1.0 -
;    .1 -
;    .2 - Button (0=pushed) (input)
;    .3 - LED (0=on)        (output)
;
;*******************************************************************************

;//$PAGE
; Directives
FillROM 0

; Microprocessor definitions
include "63x0x.inc"

;*************************************************
; Data Segment (RAM)
;*************************************************

; Program Stack
gbSysProgramStack       :equ 00h    ; [00h-1Fh] Stack 0x20h
gbSysDataStack          :equ 50h    ; [50h-6Fh] Stack 0x20h
gbSysFIFO               :equ 70h    ; [70h-7Fh] EP0 and EP1 FIFO's

; Global Interrupt
gbSysInterruptMask      :equ 20h    ; Holds the current interrupt mask

; System tickers
gbSysTick1024us         :equ 22h    ; # of 1mSec ticks
gbSysTick1024usRoll     :equ 24h    ; # of 256mSec ticks

; USB management data
gbUSBValidRqsts         :equ 25h    ; Count of USB recognized requests
                                    ;   Used during debug
gbUSBSendSequence       :equ 26h    ; Buffer send data 0/1 line
gbUSBSendBytes          :equ 27h    ; Buffer bytes left to send
gbUSBSendBuffer         :equ 28h    ; Offset into current buffer

gbSuspendCount          :equ 30h    ; # of msec bus has been IDLE

; General
gbSysEnumerated         :equ 29h    ; Device is enumerated

; LED management
gbLEDBrightnessUpdate   :equ 2Bh    ; Semaphore to reset brightness
gbLEDBrightness         :equ 2Ch    ; Current brightness
LED_ON                  :equ 08h    ; P13 is used to indicate Enumeration

; Button management
gbButtonDebounce        :equ 2Dh    ; Debounce count down value
gbButtonPushed          :equ 7Ah    ; USBEndP1FIFO +2 (toggles if button was clicked)
Button_Pin              :equ 04h    ; Pin the switch is on, P12

;Port management
gbPort0                :equ 2Eh
gbPort1               :equ 2Fh

;//$PAGE
;*************************************************
; Code Segment (ROM)
;*************************************************

; Vector Table
org 00h
    jmp main                ; Reset of some type
    jmp SysUnUsed           ; 128us timer (not used)
    jmp SysTimer1024usEvent ; 1024us timer
    jmp USBEndPoint0Event   ; EP0
    jmp SysUnUsed           ; EP1 (not used)
    jmp SysUnUsed           ; Reserved
    jmp SysGPIOEvent        ; Button
    jmp SysUnUsed           ; CExt (not used)

;*************************************************
; Unused event
; Do nothing, restore machine to prior state
;*************************************************
SysUnUsed: 
    push  a
    mov   a,[gbSysInterruptMask]
    ipret SysInterrupt

;//$PAGE
;*******************************************************************************
; main()
; @func Entry point after PowerOn, WatchDog timeout or WakeUp from sleeping.
; @comm Never returns
;*******************************************************************************
main:
    ; This portion of Main is only executed after a RESET (Power-On or USB)
    ; Setup data stack in high order RAM, just below EP0 FIFO
    ; It will grow down from here
    mov  a,70h      ; USBEndP0FIFO
    swap a,dsp

    ; Initialize both Ports high 
    mov  a,FFh
    iowr SysPort0   ; Port 0 Data reg
    iowr SysPort1   ; Port 1 Data reg 
    mov [gbPort0],a     ;neu
    mov [gbPort1],a
                    ;   1 on P13 is needed to make sure enumerate LED is off
                    ;   1 on any port that needs to be an input
    ; Enable Pullups (0=enable)
    mov  a,0
    iowr SysPort0PullUp
    mov  a,Button_Pin
    iowr SysPort1PullUp     ; 1 on P12 is needed to make sure GPIO interrupt
                            ;   occurs on LOW to HIGH transistion. This
                            ;   disables it's pull up
               
    ; Enable or disable interrupts on appropriate pins
    mov  a,0
    iowr SysPort0IntEnable     ; All pins irq's are disabled on Port 0
    mov  a,Button_Pin              
    iowr SysPort1IntEnable     ; Enable P12, the button pin.
                               ; No interrupts will occur until the device
                               ;   is enumerated. Then GPIO's will be enabled and
                               ;   we will allow P12 to generate interrupts

    ; Initialize timers
    mov  a,0
    mov  [gbSysTick1024us],a
    mov  [gbSysTick1024usRoll],a

    ; Initialize USB variables
    mov  a,0
    mov  [gbUSBValidRqsts],a    ; No valid requests yet
    mov  [gbUSBSendSequence],a  ; Start with a 0
    mov  [gbSysEnumerated],a    ; Not enumerated

    ; Initialize LED
    mov  a,1     ; flag it for an update
    mov  [gbLEDBrightnessUpdate],a
    mov  a,FFh   ; set for maximum brightness
    mov  [gbLEDBrightness],a

    ; Initialized Button
    mov  a,0
    mov  [gbButtonPushed],a     ; Initial state of 0, no button pushed

    ; Initialize variables
    mov a,0
    mov [gbUSBSendBytes],a      ; No bytes to send in FIFO buffers
    mov [gbSuspendCount],a      ; Reset bus activity to 0
    mov [gbButtonDebounce],a    ; We are not debouncing

    ; Set interrupt mask
    mov  a,SysIntTimer1024us  | SysIntUSBEndP0 
    mov  [gbSysInterruptMask],a

    ;*********************************************
  MainLoop:
    ; Enable interrupts to current mask
    mov  a,[gbSysInterruptMask]
    iowr SysInterrupt

    ;*************************
    ; do nothing until we are enumerated
    mov  a,0
    cmp  a,[gbSysEnumerated]
    jz   MainLoop                   ; Not enumerated, loop


    ; Ah! We're enumerated, lets do the rest of the loop
    ;*************************

    ; Write a 0 to the LED on P13 to turn it on
    mov a,F7h
    mov [gbPort1],a

Loop2:
    ; Enable interrupts to current mask
    mov  a,[gbSysInterruptMask]
    iowr SysInterrupt

    ;*************************
    ; do nothing until we are enumerated
    mov  a,0
    cmp  a,[gbSysEnumerated]
    jz   Loop2                  ; Not enumerated, loop

    ; Wait 10 milliseconds
    mov  a,10
    call SysDelayMS

    ;*************************
    ; Read temperature
    call ThermReadTemperature

    ;*************************
    ; Update brightness?
    ; mov  a,0
    cmp  a,[gbLEDBrightnessUpdate]
    jz   MainLoopNoLEDUpdate        ; No, branch

    ; Yes, update the LED brightness
    ; Reset the LED update flag
    ; mov a,0h
    mov  [gbLEDBrightnessUpdate],a

    ; Set new brightness
    mov  a,[gbLEDBrightness]
    iowr SysPort1ISinkPin3

    ; Fall through to here in any case
  MainLoopNoLEDUpdate:

    ; Loop
    jmp  Loop2  ;neu, alt: MainLoop
    ;*********************************************

    halt
    ; Oops! We should never get here


;********************************************************
; SysTimer1024usEvent()
; @func Timer interrupt event ocurring every 1.024 mSec
;       using 6Mhz crystal.
;********************************************************
SysTimer1024usEvent:

    ; Save accumulator
    push a

    ; Clear watchdog timer
    ; Clearing it here effectively disables the timer
    iowr SysWatchDog

  STPortsRefresh:   ;neu
    mov  a,[gbPort0]
    iowr SysPort0
    mov  a,[gbPort1]
    iowr SysPort1

    ; Keep track of length of any IDLE conditions (No bus activity)
    iord USBControl                    ; Read the USB Status and Control Reg
    and a,01h                          ; Check bit 0
    cmp a,0h
    jz Inc_Counter                     ; Hmm! No activity. Branch and keep track of it.
    iord USBControl                    ; Ah! There was activity, 
                                       ;  clear the bus activity bit
    and a,0feh
    iowr USBControl
    mov a,0                            ; Clear the suspend counter
    mov [gbSuspendCount],a
    jmp Suspend_End

  Inc_Counter:                         ; Monitor the IDLE count
    mov a,[gbSuspendCount]             ; Get # of mSec we have been IDLE
    inc a                              ; Increment the count
    mov [gbSuspendCount],a              
    cmp a,03h                          ; Has it been 3msec yet?
    jnz Suspend_End                    ; Not yet, branch
    mov a,0h                           ; Yes, clear the suspend counter
    mov [gbSuspendCount],a
    iord SysStatus
    or a,08h                           ; Set the suspend bit to cause a suspend
    iowr SysStatus                     ; We will enter the suspend state during
                                       ;   the next instruction.   

  Suspend_End:
   ; Increment the 1024 usec counter and check for rollover
    inc  [gbSysTick1024us]
    jnz  STimerNo1024usRoll     ; No

    ; Clear rollover
    mov  a,0
    mov  [gbSysTick1024usRoll],a

  STimerNo1024usRoll:
    ; Are we counting down a button debounce
    mov  a,0
    cmp  a,[gbButtonDebounce]
    jz   STimerNoDebounce       ; Not debouncing, branch

    ; Yes, we're debouncing. Let's see if we are timed out.
    dec  [gbButtonDebounce]
    mov  a,0
    cmp a,[gbButtonDebounce]

    ; has debounce timed out?
    jnz STimerNoDebounce        ; No, still debouncing, branch.

    ; The debounce timer has timed out
    ; check if the button pin is at a 1. If not, the button is either still
    ;    bouncing or still pushed
    iord SysPort1              ; check the port the button is on
    and a,Button_Pin           ; check the pin
    jnz STimerDebounceOver     ; branch if it is not pushed ; mrr

    ; Reset debounce since the button is not yet released or is bouncing
    mov  a,100
    mov  [gbButtonDebounce],a
    jnz STimerNoDebounce       ; continue waiting for debounce to end

  STimerDebounceOver:
    ; it's really ready!
    ; Toggle the button state flag to let the Windows app know that
    ;   the button has been pushed.
    mov  a,1
    xor  [gbButtonPushed],a

    ; Debounce must be over
  STimerNoDebounce:
    ; Enable interrupts and return
    mov  a,[gbSysInterruptMask]
    ipret SysInterrupt

;//$PAGE
;********************************************************
; SysGPIOEvent()
; @func General purpose port event
; @comm Which pin?
;********************************************************
SysGPIOEvent:

    ; Save accumulator
    push a

    ; Reset debounce any time we are here
    mov  a,100
    mov  [gbButtonDebounce],a

  SysGPIOButtonDebouncing:
    ; Enable interrupts and return
    mov  a,[gbSysInterruptMask]
    ipret SysInterrupt


;*******************************************************************************
;
;    This section of code responds to activity on End Point 0 and determines
;    what needs to be done.
;
;*******************************************************************************

;//$PAGE
;********************************************************
; USBEndPoint0Event()
; @func End Point zero USB event.
; @comm Default end point.
;********************************************************
USBEndPoint0Event:

    ; This code checks to see what type of packet was received 
    ;   (Setup, Out, or In) and jumps to the correct routine to decode the 
    ;   specifics. After the code to which the jump points is through, it jumps
    ;   back to USBEventEP0End.

    ; Save accumulator
    push a

    ; Is this a SETUP packet?
    iord USBEndP0RxStatus
    and  a,USBEndP0RxSetup      ; Check the setup bit
    jnz  USBEventEP0_SETUP      ; Yes it's a setup, branch

    ; Not a setup, is it an OUT packet?
   ;iord USBEndP0RxStatus
   ;and  a,USBEndP0RxOut
   ;jnz  USBEventEP0_OUT

    ; Not an OUT packet, is it an IN packet?
   ;iord USBEndP0RxStatus
   ;and  a,USBEndP0RxIn
   ;jnz  USBEventEP0_IN

  USBEventEP0_IN:
  USBEventEP0_OUT:

  USBEventEP0End:
    ; OK. We're done with the packet.
    ;   Let's enable interrupts and return
    mov  a,[gbSysInterruptMask]
    ipret SysInterrupt    ; done with EP0 irq service routine

  USBEventEP0Stall:
    ; Stall any subsequent IN's or OUT's until the
    ;       stall bit (bit 5) is cleard by an I/O write to
    ;       the  USB End Point 0 TX Configuration Register (0x10)
    ;       or any SETUP is received.
    iord USBEndP0TxConfig
    or   a,USBEndP0TxStall
    iowr USBEndP0TxConfig

    ; OK. We've set the stall condition for Endpoint 0.
    ;   Now let's complete the routine.
    jmp  USBEventEP0End

;*******************************************************************************
; 
;    We know we have received a Setup token. Now we need to parse it to
;    determine what command it is.
;
;*******************************************************************************


;//$PAGE
;*******************************************************************************
; USBEventEP0_SETUP()
; @func End point event SETUP packet handler.
; @devnote Runs in interrupt enabled context.
;********************************************************
USBEventEP0_SETUP:
    ; Well, we have a SETUP packet. Let's find out what to do.
    mov A,[gbSysInterruptMask]
    iowr SysInterrupt

    ; If we are here and are and we are processing a previous Setup, 
    ;  we need to abort the processing of the previous Setup
    mov  a,0    ; Clear any indication that we have bytes left to transfer
    mov [gbUSBSendBytes],a
 
    ; Clear EP0 RxReg (including the Setup flag)
    ; The Data toggle bit remains unchanged, however.
    mov  a,0
    iowr USBEndP0RxStatus

    ;*********************************************
    ; Setup Event
    ;*********************************************

    ; Check the request type and branch to the correct location to handle it.
    mov  a,[USBEndP0FIFO_0]

  USBEventEP0SetupTargetDeviceOUT:
    ; Target Device?
    cmp  a,USBRqstTargetDevice
    jz   USBEventEP0SetupIsSetAddress   ; Yes

  USBEventEP0SetupTargetInterfaceOUT:
    cmp  a,USBRqstTargetInterface
    jz   USBEventEP0Stall               ; Yes. Oops! We don't have an interface.

  USBEventEP0SetupTargetEndpointOUT:
    cmp  a,USBRqstTargetEndPoint
    jz   USBEventEP0Stall               ; Yes

  USBEventEP0SetupTargetDeviceIN:
    cmp  a,USBRqstTargetDevice | USBRqstTypeDirection
    jz   USBEventEP0SetupGetDescriptor  ; Yes

  USBEventEP0SetupTargetInterfaceIN:
    cmp  a,USBRqstTargetInterface | USBRqstTypeDirection
    jz   USBEventEP0Stall               ; Yes Oops! We don't have an interface.

  USBEventEP0SetupTargetEndpointIN:
    cmp  a,USBRqstTargetEndPoint | USBRqstTypeDirection
    jz   USBEventEP0Stall               ; Yes

    ; Vendor specific commands
  USBEventEP0SetupTargetVendorIN_OUT:
    ; Check request (IN packet OK, OUT packet ERR)
    mov  a,[USBEndP0FIFO_0]
    and  a,USBRqstTypeVendor | USBRqstTargetEndPoint | USBRqstTypeDirection
    cmp  a,USBRqstTypeVendor | USBRqstTargetEndPoint | USBRqstTypeDirection
    jz   USBEventEP0VendorRqst

    ; Unsupported request !!!
    jmp  USBEventEP0Stall               ; Oops! We don't support whatever
                                        ;   request was made.


;//$PAGE
;********************************************************
; USBEventEP0SetupIsSet()
; @func End point event SETUP to set address.
; @devnote Runs in interrupt enabled context.
;********************************************************
USBEventEP0SetupIsSetAddress:

    ; Set device address?
    mov  a,[USBRqstMessage]
    cmp  a,USBRqstSetAddress
    jz   USBEventEP0SetupSetAddress         ; Yes

  USBEventEP0SetupIsSetConfig:
    ; Set device configuration?
    mov  a,[USBEndP0FIFO_1]
    cmp  a,USBRqstSetConfiguration
    jz   USBEventEP0SetupSetConfig          ; Yes

    ; Unsupported set request !!!
    jmp  USBEventEP0Stall                   ; No. Stall

  ;USBEventEP0SetupIsGetDescriptor:
    mov  a,[USBRqstMessage]
    cmp  a,USBRqstGetDescriptor
    jz   USBEventEP0SetupGetDescriptor      ; Yes

    ; Unsupported get request !!!
    jmp  USBEventEP0Stall                   ; No

;********************************************************
; USBEventEP0SetupSetAddress()
; @func End point zero event SETUP to set address.
; @devnote Runs in interrupt enabled context.
; @comm
; The status token of the SetAddress is an IN. So, we send status manually.
;********************************************************
USBEventEP0SetupSetAddress:

    ; Send ACK
    call USBSendACK
    ; Now that we have been acknowleged, we actually set the address.
    ; This is different from all other commands which execute first
    ;   and then acknowlege (_________________)

    ; Remember this
    inc [gbUSBValidRqsts]

    ; Set Address
    mov  a,[USBRqstWordValueLo]
    iowr USBDeviceAddress

    ; Done
    jmp  USBEventEP0End

;********************************************************
; USBEventEP0SetupSetConfig()
; @func End point zero event SETUP to Set Configuration.
; @devnote Runs in interrupt enabled context.
; 1
;  set enumerated (gbSysEnumerated) state, 
;  enable GPIO (and EP1, if appropriate)
;  Enable P0 and P1
; 0
;  Reset enumerated (gbSysEnumerated) state, 
;  Turn off LED
;  Reset variables
;  Disable GPIO and EP1
;  Disable dallas chip and P0 and P1
;********************************************************
USBEventEP0SetupSetConfig:

    ; Enumerated !
    mov  a,01h
    mov  [gbSysEnumerated],a

    ; Initialize thermometer
    call ThermInitialize

    ; Enable button interrupt on port 1.
    ;   Actually, this has already been done in main().
    mov  a,04h
    iowr SysPort1IntEnable

    ; enable all appropriate irq's
    mov  a,SysIntTimer1024us | SysIntGPIO | SysIntUSBEndP0
    mov  [gbSysInterruptMask],a

    ; Send ACK
    call USBSendACK
    jmp  USBEventEP0End

;//$PAGE
;********************************************************
; USBEventEP0SetupGetDescriptor()
; @func End point zero event SETUP to Get Descriptor.
; @devnote Runs in interrupt enabled context.
;********************************************************
USBEventEP0SetupGetDescriptor:

    ; Get descriptor type
    mov  a,[USBRqstWordValueHi]

  USBEventEP0SetupGetDescriptorDevice:
    ; Device Descriptor?
    cmp  a,USBDescriptorTypeDevice
    jnz  USBEventEP0SetupGetDescriptorConfig    ; No

    ; Remember this
    inc [gbUSBValidRqsts]

    ;*********************************************
    ; Get Device Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBDeviceDescription -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,12h                  ;[USBDeviceDescription]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorConfig:
    ; Configuration Descriptor?
    cmp  a,USBDescriptorTypeConfig
    jnz  USBEventEP0SetupGetDescriptorString    ; No

    ; Remember this
    inc [gbUSBValidRqsts]

    ;*********************************************
    ; Get Configuration Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBConfigurationDescription -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,09h                  ;[USBConfigurationDescription]
    add  a,09h                  ;[USBInterfaceDescription]
    add  a,07h                  ;[USBEndPointDescriptionInt]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString:
    ; Get String Descriptor?
    cmp  a,USBDescriptorTypeString
    jnz  USBEventEP0SetupGetDescriptorEnd       ; No

    ;*********************************************
    ; Get String Descriptor Event
    ;*********************************************

    ; Get string descriptor index
    mov  a,[USBRqstWordValueLo]

  USBEventEP0SetupGetDescriptorString0:
    cmp  a,0h
    jnz  USBEventEP0SetupGetDescriptorString1   ; No

    ;*********************************************
    ; Get String Language(s) Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringLanguageDescription -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,4h                   ;[USBStringLanguageDescription]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString1:
    cmp  a,1
    jnz  USBEventEP0SetupGetDescriptorString2   ; No

    ;*********************************************
    ; Get String 1 Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringDescription1 -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,10h                  ;[USBStringDescription1]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString2:
    cmp  a,2
    jnz  USBEventEP0SetupGetDescriptorString3   ; No

    ;*********************************************
    ; Get String 2 Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringDescription2 -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,18h                  ;[USBStringDescription2]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString3:
    cmp  a,3
    jnz  USBEventEP0SetupGetDescriptorString4   ; No

    ;*********************************************
    ; Get String 3 Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringDescription3 -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,24h                  ;[USBStringDescription3]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString4:
    cmp  a,4
    jnz  USBEventEP0SetupGetDescriptorString5   ; No

    ;*********************************************
    ; Get String 4 Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringDescription4 -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,20h                  ;[USBStringDescription4]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorString5:
    cmp  a,5
    jnz  USBEventEP0SetupGetDescriptorEnd       ; No

    ;*********************************************
    ; Get String 5 Descriptor Event
    ;*********************************************
    ; Descriptor pointer
    mov  a,(USBStringDescription5 -USBSendROMBufferBase)
    mov  [gbUSBSendBuffer],a

    ; Descriptor size
    mov  a,3Ch                  ;[USBStringDescription5]
    mov  [gbUSBSendBytes],a

    ; Check request size field
    call USBSendDescriptorCheckLength

    ; Send buffer
    call USBSendROMBuffer
    jmp  USBEventEP0End

  USBEventEP0SetupGetDescriptorEnd:
    ; Unsupported Get request !!!
    jmp  USBEventEP0Stall

;//$PAGE
;********************************************************
; USBSendDescriptorCheckLength()
; @func Check and update send length for Get Descriptor
;       requests on end point 0.
; @parm BYTE | gbUSBSendBytes | Number of bytes to send.
;********************************************************
USBSendDescriptorCheckLength:

    ; High byte set? (Assume <255 bytes)
    mov  a,[USBEndP0FIFO_7]
    cmp  a,0
    jnz  USBSendDescriptorCheckLengthEnd    ; Yes

    ; Check size
    mov  a,[USBEndP0FIFO_6]
    cmp  a,[gbUSBSendBytes]
    jz   USBSendDescriptorCheckLengthEnd    ; equal
    jnc  USBSendDescriptorCheckLengthEnd    ; greater than

    ; New size
    mov  [gbUSBSendBytes],a

  USBSendDescriptorCheckLengthEnd:
    ret

;//$PAGE
;********************************************************
; USBSendROMBuffer()
; @func Send a number of ROM bytes on end point 0.
; @parm BYTE | gbUSBSendBytes | Number of bytes to send.
; @parm BYTE | gbUSBSendBuffer | Offset from ROM base
;       of data to send.
; @comm assumes IN packets are ignored in the interrupt routine
; @devnote Enables interrupts
;********************************************************
USBSendROMBuffer:

    ; Clear flag
    mov  a,0h
    iowr USBEndP0RxStatus

    ; Enable interrupts
    mov  a,[gbSysInterruptMask]
    and  a,~SysIntUSBEndP0
    iowr SysInterrupt

    ; Auto ACK OUT packet (This would be a Status Out)
    mov  a,USBControlAckStatusData
    iowr USBControl

    ; Initialize sequence
    mov  a,0h
    mov  [gbUSBSendSequence],a

    ; Send count
    mov  a,[gbUSBSendBytes]

USendROMBufferLoop:
    ; One 8-byte chunk or less left?
    cmp  a,08h
    jz   USendROMBufferLoopDone     ; exactly 8 bytes left, branch
    jc   USendROMBufferLoopDone     ; less than 8 bytes left, branch

    ; more than 8 bytes left, fall through and loop 
    ;    until there are 8 bytes or less.
    ; Save count
    push a

    ; Send 8 byte chunk
    mov  a,08h
    mov  [gbUSBSendBytes],a
    call _USBSendROMBuffer

    ; Check for OUT packet cancelling send
    iord USBEndP0RxStatus
    and  a,USBEndP0RxOut

    ; Restore count
    pop  a

    ; Handle exception: OUT packet cancel send
    jnz  USendROMBufferLoopExit     ; Cancelled

    ; Save bytes left
    sub  a,08h
    mov  [gbUSBSendBytes],a
    jmp  USendROMBufferLoop

  USendROMBufferLoopDone:
    ; Send last 8 or less bytes
    call _USBSendROMBuffer

USendROMBufferLoopExit:
    ret

;//$PAGE
;********************************************************
; _USBSendROMBuffer()
; @func Buffer and inialize USB send of up
;               to 8 bytes of ROM data on end point 0.
; @comm affects gbUSBSendBytes & gbUSBSendBuffer
;********************************************************
_USBSendROMBuffer:

    ; Save x
    push x

    ; Initialize
    mov  x,0h

  _USendROMBufferLoop:
    ; Any more?
    mov  a,0h
    cmp  a,[gbUSBSendBytes]
    jz  _USendROMBufferLoopDone         ; No more
    dec  [gbUSBSendBytes]

    ; Move bytes to FIFO
    mov  a,[gbUSBSendBuffer]
    index USBSendROMBufferBase
    mov  [x +USBEndP0FIFO],a
    inc  x

    ; Next byte
    inc  [gbUSBSendBuffer]
    jmp _USendROMBufferLoop

  _USendROMBufferLoopDone:
    ; Re-enable reception
    mov  a,0h
    iowr USBEndP0RxStatus

    ; Toggle sequence
    mov  a,USBEndP0TxSequence
    xor  [gbUSBSendSequence],a

    ; Send bytes
    push x
    pop  a
    or   a,[gbUSBSendSequence]
    or   a,USBEndP0TxRespond
    iowr USBEndP0TxConfig

    ; The FIFO is loaded, go and wait untill it's read
    call USBSendWaitForComplete

  _USendROMBufferEnd:
    ; Restore and exit
    pop  x
    ret

;//$PAGE
;********************************************************
; USBSendACK()
; func Respond to a "USB Status In" with a zero byte buffer with 
;   Sequence field set) on end point 0.
; Called by SetAddress and SetConfig commands
;********************************************************
USBSendACK:

    ; Status response to Status In is to send a zero byte packet
    mov  a,USBEndP0TxRespond | USBEndP0TxSequence
    iowr USBEndP0TxConfig

    ; Enable interrupts
    mov  a,[gbSysInterruptMask]
    iowr SysInterrupt

    ; Wait for send complete
    jmp  USBSendWaitForComplete

;********************************************************
; USBSendWaitForComplete()
; @func Wait for send to complete on end point 0.
;********************************************************

; At some point, either the 0 data will be ACK'd or a SETUP
;    will come in. 
; Either event will cause the "Enable Respond
;    to In Packets" to be reset, and we will fall out of the loop.
; In either case, an EP0 IRQ will be generated (5.9.2.2 in Cyp
;    device spec) if EP0 irq is enabled.

USBSendWaitForComplete:

    ; Poll the send complete bit
    ; This will be reset when the data has been sent to the host
    ;   and the host has ACK's, or the host has sent another SETUP
    ;   which should terminate this activity in any case.
    iord USBEndP0TxConfig
    and  a,USBEndP0TxRespond
    jz   USBSendWaitComplete

    ; Check for OUT packet cancelling send. A STATUS OUT should
    ;   terminate any pending IN's. A Setup could also set the Out bit.
    iord USBEndP0RxStatus
    and  a,USBEndP0RxOut
    jnz  USBSendWaitComplete    ; Cancelled

    ; Keep waiting
    jmp  USBSendWaitForComplete

  USBSendWaitComplete:
    ret

;//$PAGE
;********************************************************
; USBEventEP0VendorRqst()
; @func Vendor request on end point zero.
; @devnote Runs in interrupt disabled context.
;********************************************************
USBEventEP0VendorRqst:

    ; Save it
    push x

    ; Check Protocol
    mov  a,[USBEndP0FIFO_1]

  USBEventEP0VendorRqstPing:
    cmp  a,0h
    jnz  USBEventEP0VendorRqstReadROM       ; No

    ;*********************************************
    ; Ping Event
    ;*********************************************
    jmp  USBEventEP0VendorRqstFinish

  USBEventEP0VendorRqstReadROM:
    cmp  a,01h
    jnz  USBEventEP0VendorRqstReadRAM       ; No

    ;*********************************************
    ; Read ROM Event
    ;*********************************************
    mov  a,[USBEndP0FIFO_2]
    index USBSendROMBufferBase
    mov  [USBEndP0FIFO_1],a
    jmp  USBEventEP0VendorRqstFinish

  USBEventEP0VendorRqstReadRAM:
    cmp  a,02h
    jnz  USBEventEP0VendorRqstWriteRAM      ; No

    ;*********************************************
    ; Read RAM Event
    ;*********************************************
    mov  a,[USBEndP0FIFO_2]
    push a
    pop  x
    mov  a,[x +0]
    mov  [USBEndP0FIFO_1],a
    jmp  USBEventEP0VendorRqstFinish

  USBEventEP0VendorRqstWriteRAM:
    cmp  a,3
    jnz  USBEventEP0VendorRqstReadPort      ; No

    ;*********************************************
    ; Write RAM Event
    ;*********************************************
    mov  a,[USBEndP0FIFO_2]
    push a
    pop  x
    mov  a,[USBEndP0FIFO_4]
    mov  [x +0],a
    jmp  USBEventEP0VendorRqstFinish

  USBEventEP0VendorRqstReadPort:
    cmp  a,04h
    jnz  USBEventEP0VendorRqstWritePort     ; No

    ;*********************************************
    ; Read Port Event
    ;*********************************************
    mov  a,[USBEndP0FIFO_2]
    cmp  a,0h
    jnz  USBEventEP0VendorRqstReadPort1

  USBEventEP0VendorRqstReadPort0:
    iord SysPort0
    jmp  USBEventEP0VendorRqstReadPortsDone

  USBEventEP0VendorRqstReadPort1:
    iord SysPort1
   ;jmp  USBEventEP0VendorRqstReadPortsDone ; redundant, but good practice

  USBEventEP0VendorRqstReadPortsDone:
    mov  [USBEndP0FIFO_1],a
    jmp  USBEventEP0VendorRqstFinish

  USBEventEP0VendorRqstWritePort:
    cmp  a,05h
    jnz  USBEventEP0Stall                   ; No

    ;*********************************************
    ; Write Port Event
    ;*********************************************
    mov  a,[USBEndP0FIFO_2]
    cmp  a,0
    jnz  USBEventEP0VendorRqstReadPort1

  USBEventEP0VendorRqstWritePort0:
    mov  a,[USBEndP0FIFO_4]
    iowr SysPort0
    jmp  USBEventEP0VendorRqstWritePortsDone

  USBEventEP0VendorRqstWritePort1:
    mov  a,[USBEndP0FIFO_4]
    iowr SysPort1
   ;jmp  USBEventEP0VendorRqstWritePortsDone ; redundant, but good practice

  USBEventEP0VendorRqstWritePortsDone:
   ;jmp  USBEventEP0VendorRqstFinish ; redundant, but good practice

  USBEventEP0VendorRqstFinish:

    ; Protocol ACK
    mov  a,42h
    mov  [USBEndP0FIFO_0],a

    ; Auto ACK OUT packet
    mov  a,USBControlAckStatusData
    iowr USBControl

    ; Send bytes as Data1
    mov  a,8
    or   a,USBEndP0TxSequence
    or   a,USBEndP0TxRespond
    iowr USBEndP0TxConfig

   ;call USBSendWaitForComplete

    ; Restore it
    pop  x

    ; Return
    jmp  USBEventEP0End

;*****************************
;//$PAGE
include "ds1620a.asm"
;*****************************

;********************************************************
; SysDelayMS()
; @func Delay some number of milliseconds.
; @parm register | A | Number of milliseconds (0=65536).
; @comm Protects A and X registers.
;********************************************************
SysDelayMS:

    ; Save em'
    push a
    push x

  SysDelayMSLoop:
    ; Save count
    push a

    ; Delay 1ms = 10 * 100us
    mov  a,10

  SysDelayMSLoopDelay:
    ; Save it
    push a

    ; Delay 100us
    mov  a,100
    call SysDelay

    ; Done?
    pop  a
    dec  a
    jnz  SysDelayMSLoopDelay

    ; Done?
    pop  a
    dec  a
    jnz  SysDelayMSLoop

    ; Restore em'
    pop  x
    pop  a
    ret

;********************************************************
; SysDelay()
; @func Delay some number of microseconds.
; @parm register | A | Number of microseconds (0=65536).
; @comm Protects A and X registers.
;********************************************************
SysDelay:

    ; Save em'
    push a
    push x

  SysDelayLoop:
    ; Save count
    push a

    ; Delay 1ms
    nop     ; 4 clock cycles (6Mhz or 166us cycle???)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ; Done?
    pop  a
    dec  a
    jnz  SysDelayLoop

    ; Restore em'
    pop  x
    pop  a
    ret


;********************************************************
; Data Segment (ROM)
;********************************************************
USBSendROMBufferBase:
USBDeviceDescription:
    db 12h          ; Length
    db 01h          ; Type (1=device)
    db 00h,01h      ; Complies to USB Spec. v1.00
    db 00h          ; Class code (0=??)
    db 00h          ; SubClass code (0=??)
    db 00h          ; Protocol (0=none)(9.6.1)
    db 08h          ; Max. packet size for port0
    db B4h,04h      ; Vendor ID: (0x4B4=Cypress)
    db 02h,00h      ; Product ID (0x02=USB Thermometer)
    db 09h,00h      ; Device release v0.90
    db 01h          ; Manufacturer string descriptor index (0=none)
    db 02h          ; Product string descriptor index (0=none)
    db 00h          ; Serial number string descriptor index (0=none)
    db 01h          ; Number of possible configurations
USBDeviceDescriptionEnd:

;*************************************************
;
USBConfigurationDescription:
    db 09h          ; Length
    db 02h          ; Type (2=config)
    db 19h,00h   ; Total data length (1 config,1 interface,1 endpoints)
    db 01h          ; Interface supported (1=???)
    db 01h          ; Configuration value (1=???)
    db 04h          ; Confituration string descriptor index (0=none)
    db 80h          ; Configuration (80h=Bus powered)
    db 32h          ; Maximum power consumption in 2mA units
USBConfigurationDescriptionEnd:

;*************************************************
;
USBInterfaceDescription:
    db 09h          ; Length
    db 04h          ; Type (4=interface)
    db 00h          ; Number of interfaces (0 based)
    db 00h          ; Alternate settings
    db 01h          ; Number of endpoints (1 based) (9.6.3)
    db 00h          ; Class code (0=non-specified,1=kb,2=mouse,3=joystick ???)
    db 00h          ; Subclass code (0=???)
    db 00h          ; Protocol code (0=non-specified)
    db 05h          ; Interface string index (0=non-specified, 1,2,3,...)
USBInterfaceDescriptionEnd:

;*************************************************
; Never used for EP0
USBEndPointDescriptionInt:
    db 07h          ; Length
    db 05h          ; Type (5=endpoint)
    db 81h          ; Address (EP#=1 | [0x80=IN, 0=OUT])
    db 03h          ; Attribute (0=control,1=isochronous,2=bulk,3=interrupt)
    db 08h,00h      ; Max packet size
    db 0Ah          ; Interval (10 ms)
USBEndPointDescriptionIntEnd:

;*************************************************
;
USBStringLanguageDescription:
    db 04h          ; Length
    db 03h          ; Type (3=string)
    db 09h          ; Language:  English
    db 01h          ; Sub-language: US

USBStringDescription1:
    db 10h          ; Length
    db 03h          ; Type (3=string)
    dsu "Cypress"

USBStringDescription2:
    db 18h          ; Length
    db 03h          ; Type (3=string)
    dsu "Thermometer"

USBStringDescription3:
                    ; If a SN is used, this must be unique
                    ;    for every device or the device may
                    ;    not enumerate properly

USBStringDescription4:
    db 20h          ; Length
    db 03h          ; Type (3=string)
    dsu "Get Temperature"

USBStringDescription5:
    db 3Ch          ; Length
    db 03h          ; Type (3=string)
    dsu "EndPoint1 10ms Interrupt Pipe"

USBSendROMBufferTail:

CopyrightStrings:
    ds "USB Thermometer Project, Version 1.01"
    ds "Copyright Slade Systems, Inc., July, 1997"
    ds "Copyright Marc Reinig, July, 1997"
    ds "Copyright Cypress Semiconductors, Inc., July, 1997"

