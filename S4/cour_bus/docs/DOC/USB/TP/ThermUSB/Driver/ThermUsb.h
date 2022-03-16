////////////////////////////////////////////////////////////
//  ThermUsb.h - definition of CThermUsb
//  Copyright (c) 1998, BlueWater Systems, Inc.
////////////////////////////////////////////////////////////

    // thermometer Vendor Specific Request Codes:
#define VENDOR_READRQST_RAM   0x02
#define VENDOR_WRITERQST_RAM  0x03
#define VENDOR_READRQST_PORT  0x04
#define VENDOR_WRITERQST_PORT 0x05

    // RAM offsets for Temperature and LED brightness values
#define TEMPERATURE_READ_LOW_OFFSET     0x0078 // low 8 bits of temperature
#define TEMPERATURE_READ_HIGH_OFFSET    0x0079 // 9th bit of temperature
#define LED_BRIGHTNESS_OFFSET           0x002C // 8 bit brightness setting
#define LED_BRIGHTNESS_SEMAPHORE_OFFSET 0x002B // semaphore to update brightness
#define BUTTON_STATE_OFFSET             0x007A // button state offset

    // return packet size
#define SIZE_OF_PACKET_BUFFER 8

    // the increment applied to completed requests
#define THERMUSB_IO_INCREMENT           IO_NO_INCREMENT

    // declare the AddDevice function to be Extern "C" so that the alloc_text
    // pragma won't get confused by the name decoration
extern "C" NTSTATUS CALLBACK ThermUsbAddDevice(PDRIVER_OBJECT pDriverObject,
                                               PDEVICE_OBJECT pPhysicalDeviceObject);

    // the class definition
class CThermUsb : public CUsb
{
public:
        // constructor for CThermUsb device object
    CThermUsb(      PDRIVER_OBJECT    pDriverObject,
              CONST PDEVICE_OBJECT    pPhysicalDeviceObject,
              CONST PWCHAR            pDeviceName,
              CONST ULONG             DeviceType,
                    PUNICODE_STRING   pRegistryPath,
              CONST ULONG             DeviceNumber,
                    PDEVICE_OBJECT    *ppDeviceObject,
                    PVOID             pContext);

        // destructor
    ~CThermUsb();

        // driver wide DeviceIoControl dispatch routine
    friend NTSTATUS CALLBACK ThermUsbIoctlDispatch(
        PDEVICE_OBJECT pDeviceObject,
        PIRP           pIrp);

        // StartIo routine that all queued requests pass through
    friend VOID CALLBACK ThermUsbStartIo(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp);

        // PnP add device entry point
    friend NTSTATUS CALLBACK ThermUsbAddDevice(
        PDRIVER_OBJECT pDriverObject,
        PDEVICE_OBJECT pPhysicalDeviceObject);

        // Check the parameters for the SetLedBrightness request
    NTSTATUS SetLedBrightnessCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the SetLedBrightness request from StartIo
    NTSTATUS ThermUsbIoctlSetLedBrightness(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the GetTemperature request
    NTSTATUS GetTemperatureCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the GetTemperature request from StartIo
    NTSTATUS ThermUsbIoctlGetTemperature(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the ReadRam request
    NTSTATUS ReadRamCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the ReadRam request from StartIo
    NTSTATUS ThermUsbIoctlReadRam(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the WriteRam request
    NTSTATUS WriteRamCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the WriteRam request from StartIo
    NTSTATUS ThermUsbIoctlWriteRam(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the ReadPort request
    NTSTATUS ReadPortCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the ReadPort request from StartIo
    NTSTATUS ThermUsbIoctlReadPort(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the WritePort request
    NTSTATUS WritePortCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Perform the WritePort request from StartIo
    NTSTATUS ThermUsbIoctlWritePort(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // Check the parameters for the GetButtonState request
    NTSTATUS GetButtonStateCheckParameters(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

      // Perform the GetButtonState request from StartIo
    NTSTATUS ThermUsbIoctlGetButtonState(
        PIRP                pIrp,
        PIO_STACK_LOCATION  pIrpStack);

        // multi-purpose completion routine for getting data from
        // the buffer
    static NTSTATUS ThermUsbCompletionGetData(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp,
        PVOID           pContext);

        // completion routine for getting the low byte of the temp
    static NTSTATUS ThermUsbCompletionGetTempLow(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp,
        PVOID           pContext);

        // completion routine for getting the high byte of the temp
    static NTSTATUS ThermUsbCompletionGetTempHigh(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp,
        PVOID           pContext);

        // completion routine for setting the brightness value
    static NTSTATUS ThermUsbCompletionSetBrightness(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp,
        PVOID           pContext);

        // completion routine for setting the LED semaphore
    static NTSTATUS ThermUsbCompletionSetLedSemaphore(
        PDEVICE_OBJECT  pDeviceObject,
        PIRP            pIrp,
        PVOID           pContext);

        // device dependent call used on power down (IRP_MN_SET_POWER)
        // called by OnPowerSetPower handler -- called only when device
        // power IRP received (as opposed to system) -- called only
        // if a transition occurs from PowerSystemWorking to a state
        // greater than PowerSystemWorking
        // power information is stored in pIrpStack->Parameters.Power
    virtual NTSTATUS SavePowerRegisters(PDEVICE_OBJECT     pDeviceObject,
                                        PIRP               pIrp,
                                        DEVICE_POWER_STATE DeviceStateEntering,
                                        SYSTEM_POWER_STATE SystemStateEntering)
    {
        ULONG returnLength;                     // the length of the data returned
        USHORT brightnessControlValue = 0x0;    // control value for setting the brightness
        NTSTATUS status = STATUS_SUCCESS;       // status return

            // TODO: Add power down code here

            // demonstration -- LED will dim on power down call
            // The brightness control is accessed by first setting the brightness
            // with a Write request to RAM using
            // the Vendor request to endpoint funtion
            // The REQUEST field is set to VENDOR_WRITERQST_RAM
            // The VALUE field is set to RAM offset denoted by LED_BRIGHTNESS_OFFSET
            // The INDEX Field contains the Data to be written
        status = SynchronousClassOrVendorRequest(
                               &m_PacketBuffer,
                               SIZE_OF_PACKET_BUFFER,
                               URB_FUNCTION_VENDOR_ENDPOINT,
                               VENDOR_WRITERQST_RAM,
                               LED_BRIGHTNESS_OFFSET,
                               brightnessControlValue,
                               FALSE,
                               &returnLength);

        if (!NT_SUCCESS(status)) {
            KdPrint(("THERMUSB: CThermUsb::SavePowerRegisters: SynchronousClassOrVendorRequest (control) failed 0x%08X\n",
                status));
            return STATUS_SUCCESS;
        }

            // set LED change brightness semaphore
        status = SynchronousClassOrVendorRequest(
                            &m_PacketBuffer,
                            SIZE_OF_PACKET_BUFFER,
                            URB_FUNCTION_VENDOR_ENDPOINT,
                            VENDOR_WRITERQST_RAM,
                            LED_BRIGHTNESS_SEMAPHORE_OFFSET,
                            0xFF,
                            FALSE,
                            &returnLength);

        if (!NT_SUCCESS(status)) {
            KdPrint(("THERMUSB: CThermUsb::SavePowerRegisters: SynchronousClassOrVendorRequest failed: 0x%08X\n",
                status));
            return STATUS_SUCCESS;
        }

            // only want to return status success
        return STATUS_SUCCESS;
    }

        // device dependent call used on power up (IRP_MN_SET_POWER)
        // called by OnPowerSetPower handler -- called only when device
        // specific IRP received (as opposed to system) -- called only
        // when a transition occurs from a state greater than
        // PowerSystemWorking to a state <= PowerSystemWorking.
        // power information is stored in pIrpStack->Parameters.Power
    virtual NTSTATUS RestorePowerRegisters(PDEVICE_OBJECT     pDeviceObject,
                                           PIRP               pIrp,
                                           DEVICE_POWER_STATE DeviceStateLeaving,
                                           SYSTEM_POWER_STATE SystemStateLeaving)
    {
        ULONG returnLength;                     // the length of the data returned
        USHORT brightnessControlValue = 0xff;   // control value for setting the brightness
        NTSTATUS status = STATUS_SUCCESS;       // status return

            // TODO: Add power down code here

            // demonstration -- LED will dim on power down call
            // The brightness control is accessed by first setting the brightness
            // with a Write request to RAM using
            // the Vendor request to endpoint funtion
            // The REQUEST field is set to VENDOR_WRITERQST_RAM
            // The VALUE field is set to RAM offset denoted by LED_BRIGHTNESS_OFFSET
            // The INDEX Field contains the Data to be written
        status = SynchronousClassOrVendorRequest(
                               &m_PacketBuffer,
                               SIZE_OF_PACKET_BUFFER,
                               URB_FUNCTION_VENDOR_ENDPOINT,
                               VENDOR_WRITERQST_RAM,
                               LED_BRIGHTNESS_OFFSET,
                               brightnessControlValue,
                               FALSE,
                               &returnLength);

        if (!NT_SUCCESS(status)) {
            KdPrint(("THERMUSB: CThermUsb::RestorePowerRegisterss: SynchronousClassOrVendorRequest (control) failed: 0x%08X\n",
                status));
            return STATUS_SUCCESS;

        }

            // set LED change brightness semaphore
        status = SynchronousClassOrVendorRequest(
                            &m_PacketBuffer,
                            SIZE_OF_PACKET_BUFFER,
                            URB_FUNCTION_VENDOR_ENDPOINT,
                            VENDOR_WRITERQST_RAM,
                            LED_BRIGHTNESS_SEMAPHORE_OFFSET,
                            0xFF,
                            FALSE,
                            &returnLength);

        if (!NT_SUCCESS(status)) {
            KdPrint(("THERMUSB: CThermUsb::RestorePowerRegisterss: SynchronousClassOrVendorRequest (control) failed: 0x%08X\n",
                status));
            return STATUS_SUCCESS;

        }

        return STATUS_SUCCESS;
    }

public:
    UCHAR   m_PacketBuffer[SIZE_OF_PACKET_BUFFER]; // buffer to hold the ACK packet
};
