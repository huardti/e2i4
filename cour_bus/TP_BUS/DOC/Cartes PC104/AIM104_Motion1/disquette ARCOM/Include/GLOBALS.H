/****************************************************************************/
/*                                                                          */
/* Software Title: PC104 Library for Arcom PC104 products       				 */
/* Module Name:    globals.h                                                */
/*                 Copyright (c) 1996 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        globals.h - Constants and directives common to all       */
/*                             modules.                                     */
/*                                                                          */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.5 / V3.1                            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 07/02/96 1.9     serport.baud altered from unsigned short  AJL           */
/*                  to unsigned long for baud = 115200                      */
/* 04/12/96 1.8     Modified for AIM-104 v2i0                 AJL           */
/* 12/11/96 1.7     188EB/386EX DEFINITIONS COMBINED          AJL           */
/*                  IRQxx MODIFIED TO INTxx                                 */
/* 22/10/96 1.6     RTC Code Inserted                         AJL           */
/* 26/09/96 1.5      Extraneous code (eg. RTC, Interrupts)    AJL           */
/*                   removed for release                                    */
/* 16/08/96 1.4     Corrected board struct. for MULTIO IN     Alex Lennon   */
/* 15/08/96 1.3     Added defines for LCD Driver              Alex Lennon   */
/* 05/08/95 1.2     Added defines for AIM104-SER4             Alex Lennon   */
/* 25/07/96 1.1      Modified to include RTC board            Alex Lennon   */
/* 04/07/96 1.0      First release                            Graeme Rimmer */
/****************************************************************************/


#include <stdio.h>						// Needed for NULL definition

#define MAX_BOARDS      16           // Maximum number of boards allowed
#define KNOWN_BOARDS     10          // Board types known to the system


/* ---- Names for bit values ---- */
#define BIT_0			0x01
#define BIT_1			0x02
#define BIT_2			0x04
#define BIT_3			0x08
#define BIT_4			0x10
#define BIT_5			0x20
#define BIT_6			0x40
#define BIT_7			0x80


#ifdef TARGET188EB

 /* ---- Target 188EB Processor interrupt numbers for hardware IRQs ---- */

 #define INT_0        0x0C
 #define INT_1        0x0D
 #define INT_2        0x0E
 #define INT_3        0x0F
 #define INT_4        0x11

 /* ---- Interrupt controller specific (188EB) */

 #define INT0_IMR       0xED        // 8259 PIC Interrupt mask bits
 #define INT1_IMR       0xDD
 #define INT2_IMR       0xBD
 #define INT3_IMR       0x7D
 #define INT4_IMR       0xF5
 #define NINT0_IMR      ((0xFF-INT0_IMR) & 0xFD)
 #define NINT1_IMR      ((0xFF-INT1_IMR) & 0xFD)
 #define NINT2_IMR      ((0xFF-INT2_IMR) & 0xFD)
 #define NINT3_IMR      ((0xFF-INT3_IMR) & 0xFD)
 #define NINT4_IMR      ((0xFF-INT4_IMR) & 0xFD)

 #define PCB             0xFF00
 #define IMR             PCB+0x08        // Interrupt Mask Register
 #define EOIR            PCB+0x02        // End Of Interrupt Register.
 #define NS_EOI          0x8000          // Non-Specific EOI (value)

 #define ICON0           PCB+0x18
 #define ICON1           PCB+0x1A
 #define ICON2           PCB+0x1C
 #define ICON3           PCB+0x1E
 #define ICON4           PCB+0x16

 #define PRIMSK          PCB+0x0A

#endif

#ifdef LOCAL //*****************************************************

 /* ---- PC Processor interrupt numbers for hardware IRQs ---- */
 /* These are dependent on the PC base vector offset defined (0x08)   */
 /* Only defined for MASTER ICU													 */

 #define S_BV         0x08  // Master base vector - PC compatible for IRQ0-7

 #define INT_0        S_BV
 #define INT_1        S_BV+1
 #define INT_2        S_BV+2
 #define INT_3        S_BV+3
 #define INT_4        S_BV+4
 #define INT_5        S_BV+5
 #define INT_6        S_BV+6
 #define INT_7        S_BV+7


 /* ---- Interrupt controller specific (PC) */

 #define INT0_IMR       0xFE        // IMR PIC Interrupt mask bits
 #define INT1_IMR       0xFD
 #define INT2_IMR       0xFB
 #define INT3_IMR       0xF7
 #define INT4_IMR       0xEF
 #define INT5_IMR       0xDF
 #define INT6_IMR       0xBF
 #define INT7_IMR       0x7F
 #define NINT0_IMR      (0xFF-INT0_IMR)
 #define NINT1_IMR      (0xFF-INT1_IMR)
 #define NINT2_IMR      (0xFF-INT2_IMR)
 #define NINT3_IMR      (0xFF-INT3_IMR)
 #define NINT4_IMR      (0xFF-INT4_IMR)
 #define NINT5_IMR      (0xFF-INT5_IMR)
 #define NINT6_IMR      (0xFF-INT6_IMR)
 #define NINT7_IMR      (0xFF-INT7_IMR)

 #define M_ICW1          0x020
 #define M_ICW2          0x021
 #define M_ICW3          0x021
 #define M_ICW4          0x021
 #define M_OCW1          0x021
 #define M_OCW2          0x020
 #define M_OCW3          0x020

 #define IMR             M_OCW1        // Interrupt Mask Register
 #define EOIR            M_OCW2        // End Of Interrupt Register.
 #define NS_EOI          0x20          // Non-Specific EOI (value)

#endif

#ifdef TARGET386EX

 /* ---- Target 386EX Processor interrupt numbers for hardware IRQs ---- */
 /* These are dependent on the 386EX base vector offset defined (0x70)   */
 /* Only defined for SLAVE ICU														 */

 #define S_BV         0x70  // Slave base vector - PC compatible for IRQ8-15

 #define INT_0        S_BV
 #define INT_1        S_BV+1
 #define INT_2        S_BV+2
 #define INT_3        S_BV+3
 #define INT_4        S_BV+4
 #define INT_5        S_BV+5
 #define INT_6        S_BV+6
 #define INT_7        S_BV+7

 /* ---- Interrupt controller specific (386EX) */

 #define INT0_IMR       0xFE        // IMR PIC Interrupt mask bits
 #define INT1_IMR       0xFD
 #define INT2_IMR       0xFB
 #define INT3_IMR       0xF7
 #define INT4_IMR       0xEF
 #define INT5_IMR       0xDF
 #define INT6_IMR       0xBF
 #define INT7_IMR       0x7F
 #define NINT0_IMR      (0xFF-INT0_IMR)
 #define NINT1_IMR      (0xFF-INT1_IMR)
 #define NINT2_IMR      (0xFF-INT2_IMR)
 #define NINT3_IMR      (0xFF-INT3_IMR)
 #define NINT4_IMR      (0xFF-INT4_IMR)
 #define NINT5_IMR      (0xFF-INT5_IMR)
 #define NINT6_IMR      (0xFF-INT6_IMR)
 #define NINT7_IMR      (0xFF-INT7_IMR)

 #define M_ICW1          0x020
 #define M_ICW2          0x021
 #define M_ICW3          0x021
 #define M_ICW4          0x021
 #define M_OCW1          0x021
 #define M_OCW2          0x020
 #define M_OCW3          0x020

 #define S_ICW1          0x0A0
 #define S_ICW2          0x0A1
 #define S_ICW3          0x0A1
 #define S_ICW4          0x0A1
 #define S_OCW1          0x0A1
 #define S_OCW2          0x0A0
 #define S_OCW3          0x0A0

 #define IMR             S_OCW1        // Interrupt Mask Register
 #define EOIR            S_OCW2        // End Of Interrupt Register.
 #define NS_EOI          0x20          // Non-Specific EOI (value)

#endif

/* ---- These are RTC specific ---- */

#define RTC_BOARD_PNTR 1
#define RTC_BOARD_DATA 0

/* RTC Address locations */

#define RTC_SECOND			0X00		// Define address locations
#define RTC_ALM_SECOND      0X01        // for rtc
#define RTC_MINUTE			0x02
#define RTC_ALM_MINUTE		0X03
#define RTC_HOUR            0X04
#define RTC_ALM_HOUR        0X05
#define RTC_DAY_OF_WEEK		0X06
#define RTC_DATE_OF_MONTH	0X07
#define RTC_MONTH           0X08
#define RTC_YEAR            0X09

#define RTC_REG_A           0X0A        // Define register locations
#define RTC_REG_B           0X0B        // for rtc
#define RTC_REG_C           0X0C
#define RTC_REG_D           0X0D

#define RTC_DATA_START		0X0E		// Define start of user data area

#define RTC_UIP				0x80		// Define register A bits
#define RTC_DV2				0x40
#define RTC_DV1				0x20
#define RTC_DV0				0x10
#define RTC_RS3				0x08
#define RTC_RS2				0x04
#define RTC_RS1				0x02
#define RTC_RS0				0x01

#define RTC_SET				0x80		// Define register B bits
#define RTC_PIE				0x40
#define RTC_AIE				0x20
#define RTC_UIE				0x10
#define RTC_SQWE            0x08
#define RTC_DM              0x04
#define RTC_T2412           0x02
#define RTC_DSE				0x01

#define RTC_IRQF            0x80        // Define register C bits
#define RTC_PF              0x40
#define RTC_AF              0x20
#define RTC_UF              0x10

#define RTC_VRT				0x80		// Define register D bits

/* Alarm type definitions */
/* NB eg. to set don't care states for an interrupt every minute */
/* must set two highest bits of BOTH alarm minute register AND   */
/* alarm hour register                                           */
#define PER_DAY				0x01
#define PER_HOUR            0x02
#define PER_MINUTE			0x03
#define PER_SECOND			0x04

/* ---- LCD Driver specific ---- */

#define LCD_SET_UP			1
#define LCD_CLEAR           2
#define LCD_HOME            3
#define LCD_RW_CUR_LEFT		4
#define LCD_RW_CUR_RIGHT	5
#define LCD_RW_DIS_LEFT		6
#define LCD_RW_DIS_RIGHT    7
#define LCD_DISPLAY_OFF		8
#define LCD_DISPLAY_ON		9
#define LCD_CURSOR_ON		10
#define LCD_CURSOR_FLASH	11
#define LCD_CURSOR_LEFT		12
#define LCD_CURSOR_RIGHT	13
#define LCD_DISPLAY_LEFT	14
#define LCD_DISPLAY_RIGHT	15

/* ---- These are SER4 specific ---- */

#define BUFLEN              512     // 1/2K Rx buffer
#define MAXSERPORTS           8     // Max of 8 ports per board
#define MAXSERIAL             4     // Maximum number of serial boards in the system
#define SER_XMIT_TIMEOUT	0xFFFF		// Transmitter clear timeout

// Register offsets for SER4 board
#define SERREG_CH1_STAT 0x00        // Channel #0 Control(W) and Status(R)
#define SERREG_CH1_DATA 0x01        // Channel #0 Data(R/W)

#define SERREG_CH2_STAT 0x02        // Channel #1 Control(W) and Status(R)
#define SERREG_CH2_DATA 0x03        // Channel #1 Data(R/W)

#define SERREG_CH3_STAT 0x04        // Channel #2 Control(W) and Status(R)
#define SERREG_CH3_DATA 0x05        // Channel #2 Data(R/W)

#define SERREG_CH4_STAT 0x06        // Channel #3 Control(W) and Status(R)
#define SERREG_CH4_DATA 0x07        // Channel #3 Data(R/W)

#define SERREG_INT_ACKNOWLEDGE 0x08 //Interrupts acknowledge register

/* ---- LCD Driver Specific ---- */

/* ---- ADC conversion complete return code ---- */
#define ADC_COMPLETE	0x01
#define SINGLE_ENDED 0x00
#define DIFFERENTIAL 0x01

/* ---- Maximum 12 bit DAC ouput ---- */
#define DAC_MAX_VAL				4095		// Max. output for 12 bit DACs

/* ---- Board ID codes ---- */
#define ID_IO32      0x01        // Dummy identity code - IO32 doesn't return anything
#define ID_IN16      0x02        // Ditto for IN16
#define ID_OUT16     0x03        // Ditto for OUT16
#define ID_RELAY8    0x04        // Ditto for RELAY8/IN8
#define ID_SER4      0x05        // Ditto for SER4
#define ID_MULTIO    0x06        // Ditto for MULTIO
#define ID_ADC16     0x07        // Ditto for ADC16
#define ID_DAC2      0x08        // Ditto for DAC2
#define ID_RTC       0x09        // Ditto for RTC

/* ---- Error codes ---- */
#define ER_TOO_MANY             -1      // Too many boards in the system
#define ER_NO_SUCH_BOARD		-2		// A non-existant board ID has been used
#define ER_UNSUPPORTED_FUNC 	-3		// Impossible function (eg. Analog in on PCIB40)
#define ER_BAD_PORT_NUMBER		-4		// Bad port (eg. port 6 on a board with 4 ports)
#define ER_WRONG_BASE_ADDR		-5		// Setup has been called with a wrong base addr
#define ER_INDETERMINATE		-6		// An input port has an indeterminate (unsmooth) state
#define ER_INTERRUPTS_IN_USE	-7		// An attempt was made to enable interrupts on 2 boards
#define ER_BOARD_NOT_OPEN		-8		// Tried to use board that was not opened
#define ER_VALUE_OUT_OF_RANGE	-9		// Value is out of range
#define ER_USED_BY_INT			-10		// Cannot access function, board is operating on interrupts
#define ER_TIMEOUT				-11		// A function has timed out, and thus could not complete
#define ER_HARDWARE_ERROR		-12		// Hardware itself has detected an error
#define ER_BUFFER_FULL			-13		// Serial buffer full
#define ER_NO_VRT               -14     // RTC valid ram and time flag not set
#define ER_RTC_INITIALISED      -15     // RTC oscillator already set up
#define ER_UNKNOWN_ALARM        -16     // RTC alarm type unknown
#define ER_WRONG_TIME_FORMAT    -17     // RTC time storage not 24 hour or not binary
#define ER_UNKNOWN_PERIOD       -18     // Period specified not available on RTC board
#define ER_ALARM_INT_NOT_SET    -19     // An attempt has been made to reset alarm int enable register
#define ER_PERIODIC_INT_NOT_SET -20     // An attempt has been made to reset periodic int enable register
                                        //    when not already set
#define ER_UPENDED_INT_NOT_SET  -21     // An attempt has been made to reset upended int enable register
                                        //    when not already set
#define ER_NOT_RTC_INT          -22     // The interrupt being checked was not generated by the RTC
#define ER_BLOCK_TOO_LARGE      -23     // Size of data block to be written to RTC > 114 bytes
#define ER_LCD_UNKNOWN_COMMAND  -24     //  Unknown command type to send to LCD driver
#define ER_INTERRUPTS_NOT_IN_USE -25    // An attempt has been made to disable an interrupt
																		  // which is not enabled
#define ER_UNCONNECTED_IRQ      -26     // The IRQ on the AIM-104 board being used is not connected to the
																		  // target board being used
// Borland version
struct _boardstore
{
	unsigned char   board_ID;			// Type of board (read from board)
	unsigned short	base_addr;			// Base address of board
	unsigned char	int_used;			// Which IRQ is used by the board, eg.11
	void interrupt	far (*old_vect)();	// Storage for old IRQ vector
	unsigned char	dig_out_num;		// Number of digital out ports
	unsigned char   dig_out_offset;		// Pointer to 1st digital out
	unsigned char	dig_in_num;			// Number of digital in ports
	unsigned char   dig_in_offset;		// Pointer to 1st digital in
    unsigned char   enable_offset;      // Pointer to digital enable offset
    unsigned char   enable_value;       // Value to write to enable register to enable outputs
	unsigned char	ana_out_num;		// Number of analog out ports
	unsigned char   ana_out_offset;		// Pointer to 1st analog out
	unsigned char	ana_in_num;			// Number of analog in ports
	unsigned char   ana_in_offset;		// Pointer to 1st analog in
	unsigned char	has_count;			// Board uses 8254 counters
	unsigned char   count_offset;       // Offset to counter registers
	unsigned char 	int_reading;		// If, set board inputs are read on interrupt
	unsigned char	ser_num;			// Number of serial channels on this board
};


// **** Serial ports
// A 'serial' port has a ringbuffer with pointers, and setup info
// 1st physical serial channel on a board uses ringbuffer 0
struct _serport
{
	unsigned char inUse;		  			// Has this port been setup?

    unsigned char ringBuffer[BUFLEN+1];     // Ring buffer itself
	unsigned short ringHead;		  		// Head ptr
	unsigned short ringTail;		  		// Tail ptr
	int  ringErrorStatus;					// !0 = Error

    unsigned long baud;                    // Setup info, baud rate
	char parity;							// Parity - 'o', 'e', or 'n'
	unsigned char databits;					// Data bits - 7 or 8
	unsigned char stopbits;					// Stop bits - 1 or 2
};

// A board has a handle, and can have a number of serial ports
struct _serboard
{
	int					handle;

	struct _serport 	serport[MAXSERPORTS];
};

// The whole serial system holds many ports for many boards
struct _serbuf
{
	struct _serboard	board[MAXSERIAL];
	unsigned char		numboards;
};


/* - Global Variable Declarations ----------------------------------------- */

#ifdef MAIN


	struct _boardstore knownboards[KNOWN_BOARDS] =
	{
	  //ID            AD IRQ     DO    DI    EO    AO   AI    TIM  UI SER
	  { ID_IO32,      0, 0,NULL, 4,0,  4,0,  4,1,  0,0, 0,0,  0,0, 0, 0 },
	  { ID_IN16,      0, 0,NULL, 0,0,  2,0,  0,0,  0,0, 0,0,  0,0, 0, 0 },
	  { ID_OUT16,     0, 0,NULL, 2,0,  0,0,  2,3,  0,0, 0,0,  0,0, 0, 0 },
	  { ID_RELAY8,    0, 0,NULL, 1,0,  1,1,  1,1,  0,0, 0,0,  0,0, 0, 0 },
	  { ID_SER4,      0, 0,NULL, 0,0,  0,0,  0,0,  0,0, 0,0,  0,0, 0, 4 },
	  { ID_MULTIO,    0, 0,NULL, 0,0,  1,0,  0,0,  2,2, 8,2,  0,0, 0, 0 },
	  { ID_ADC16,     0, 0,NULL, 0,0,  1,0,  0,0,  0,0, 8,2,  0,0, 0, 0 },
	  { ID_DAC2,      0, 0,NULL, 0,0,  1,0,  0,0,  2,2, 0,0,  0,0, 0, 0 },
      { ID_RTC,       0, 0,NULL, 0,0,  0,0,  0,0,  0,0, 0,0,  0,0, 0, 0 },
	};

	struct _boardstore boardstore[MAX_BOARDS];
	int boardindex = 0;					// Offset of next free array entry
	int in_int;						// Indicates an interrupt is in progress
									// Analog reading routines check to see if requested board
									// is under the control of interrupts. If it is and this flag
									// is not set then function returns error.

	// Structure to hold all serial comms channels
	struct _serbuf serbuf;

#else

	extern struct _boardstore knownboards[KNOWN_BOARDS];
	extern struct _boardstore boardstore[MAX_BOARDS];
	extern int boardindex;
	extern int in_int;
	extern struct _serbuf serbuf;

#endif




