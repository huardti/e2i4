/////////////////////////////////////////////////////////////////
//  ioctlrw.cpp - user interface functions
//  Copyright (c) 1998, BlueWater Systems, Inc.
/////////////////////////////////////////////////////////////////

    // main WinDK header file
#include "WinDK.h"
    // WinDK USB extensions
#include "CUsb.h"
    // user interface and ioctl codes
#include "DDThermU.h"
    // driver defines and class definition
#include "ThermUsb.h"

///////////////////////////////////////////////////////////////////
//  SetLedBrightnessCheckParameters - validates parameters for the 
//                                    user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::SetLedBrightnessCheckParameters(PIRP                pIrp,
                                                    PIO_STACK_LOCATION  pIrpStack)
{
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength !=
        sizeof(USHORT)) {
        KdPrint(("THERMUSB: CThermUsb::SetLedBrightnessCheckParameters: Input Buffer Length was incorrect %d != sizeof(USHORT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength != 0) {
        KdPrint(("THERMUSB: CThermUsb::SetLedBrightnessCheckParameters: Output Buffer Length was incorrect %d != 0. returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    
    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlSetLedBrightness - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlSetLedBrightness(PIRP                pIrp,
                                                  PIO_STACK_LOCATION  pIrpStack)
{
    USHORT      brightnessControlValue; // brightness setting
    PUSHORT     pBuffer;                // pointer to user buffer
    NTSTATUS    status;                 // status return code

    pIrp->IoStatus.Information = 0;
    pBuffer = (PUSHORT)(pIrp->AssociatedIrp.SystemBuffer);
    brightnessControlValue = *pBuffer; 

        // copy our parameters to the parameters for the lower-level
        // USBD driver
    IoCopyCurrentIrpStackLocationToNext(pIrp);

        // The brightness control is accessed by first setting the brightness
        // with a Write request to RAM using 
        // the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_WRITERQST_RAM 
        // The VALUE field is set to RAM offset denoted by LED_BRIGHTNESS_OFFSET
        // The INDEX Field contains the Data to be written
    status = AsynchronousClassOrVendorRequest(
                           &m_PacketBuffer,
                           SIZE_OF_PACKET_BUFFER,
                           URB_FUNCTION_VENDOR_ENDPOINT,
                           VENDOR_WRITERQST_RAM,
                           LED_BRIGHTNESS_OFFSET,
                           brightnessControlValue,
                           FALSE,
                           pIrp,
                           (PIO_COMPLETION_ROUTINE)ThermUsbCompletionSetBrightness); 
    
     return status;
}

///////////////////////////////////////////////////////////////////
//  GetTemperatureCheckParameters - validates parameters for the user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::GetTemperatureCheckParameters(PIRP                pIrp,
                                                  PIO_STACK_LOCATION  pIrpStack)
{
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 0) {
        KdPrint(("THERMUSB: CThermUsb::GetTemperatureCheckParameters: Input Buffer Length was incorrect %d != 0. returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength !=
        sizeof(USHORT)) {
        KdPrint(("THERMUSB: CThermUsb::GetTemperatureCheckParameters: Output Buffer Length was incorrect %d != sizeof(USHORT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    
    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlGetTemperature - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlGetTemperature(PIRP                pIrp,
                                                PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS status; 

    pIrp->IoStatus.Information = 0;

        // copy our parameters to the parameters for the lower-level
        // USBD driver
    IoCopyCurrentIrpStackLocationToNext(pIrp);

        // The Temperature is read by sending a read request to RAM using a
        // Vendor request to endpoint funtion   
        // The REQUEST field is set to VENDOR_READRQST_RAM 
        // The VALUE field is set to RAM offset denoted by
        // TEMPERATURE_READ_LOW_OFFSET
        // The INDEX Field is not used, set to 0.
        // Read the low 8-bit value
    status = AsynchronousClassOrVendorRequest(
                         &m_PacketBuffer,
                         SIZE_OF_PACKET_BUFFER,
                         URB_FUNCTION_VENDOR_ENDPOINT,
                         VENDOR_READRQST_RAM,
                         TEMPERATURE_READ_LOW_OFFSET,
                         0,
                         FALSE,
                         pIrp,
                         (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetTempLow);    
     
    return status;
}

///////////////////////////////////////////////////////////////////
//  ReadRamCheckParameters - validates parameters for the user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ReadRamCheckParameters(PIRP                pIrp,
                                           PIO_STACK_LOCATION  pIrpStack)
{
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 
        sizeof(USHORT)) {
        KdPrint(("THERMUSB: CThermUsb::ReadRamCheckParameters: Input Buffer Length was incorrect %d != sizeof(USHORT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength !=
        sizeof(UCHAR)) {
        KdPrint(("THERMUSB: CThermUsb::ReadRamCheckParameters: Output Buffer Length was incorrect %d != sizeof(UCHAR). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }

    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlReadRam - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlReadRam(PIRP                pIrp,
                                         PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS    status;         // status return
    PUSHORT     pBuffer;        // pointer to user buffer
    USHORT      ramOffset;      // ram offset to read
     
    pIrp->IoStatus.Information = 0;   
    pBuffer   = (PUSHORT)(pIrp->AssociatedIrp.SystemBuffer);
    ramOffset = (USHORT)(*pBuffer); 

        // The RAM is read by performing a read request to RAM
        // using the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_READRQST_RAM 
        // The VALUE field is set to RAM offset passed by the user 
        // The INDEX Field is not used, set to 0.
    status = AsynchronousClassOrVendorRequest(
                           &m_PacketBuffer,
                           SIZE_OF_PACKET_BUFFER,
                           URB_FUNCTION_VENDOR_ENDPOINT,
                           VENDOR_READRQST_RAM,
                           ramOffset,
                           0,
                           FALSE,
                           pIrp,
                           (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetData); 

    return status;
}

///////////////////////////////////////////////////////////////////
//  WriteRamCheckParameters - validates parameters for the user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::WriteRamCheckParameters(PIRP                pIrp,
                                            PIO_STACK_LOCATION  pIrpStack)
{
         // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 
        sizeof(RAM_WRITE_STRUCT)) {
        KdPrint(("THERMUSB: CThermUsb::WriteRamCheckParameters: Input Buffer Length was incorrect %d != sizeof(RAM_WRITE_STRUCT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength != 0) {
        KdPrint(("THERMUSB: CThermUsb::WriteRamCheckParameters: Output Buffer Length was incorrect %d != 0. returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }

    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlWriteRam - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlWriteRam(PIRP                pIrp,
                                          PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS               status;         // status return
    PRAM_WRITE_STRUCT      pBuffer;        // pointer to user buffer
    USHORT                 ramOffset;      // ram offset to write
    USHORT                 ramData;        // ram data

    pIrp->IoStatus.Information = 0;
    pBuffer = (PRAM_WRITE_STRUCT)(pIrp->AssociatedIrp.SystemBuffer);
    ramOffset = pBuffer->Address;
    ramData = pBuffer->Data;

        // The RAM is written by performing a write request to RAM
        // using the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_WRITERQST_RAM 
        // The VALUE field is set to RAM offset passed by the user 
        // The INDEX field contains the Data to write
    status = AsynchronousClassOrVendorRequest(
                           &m_PacketBuffer,
                           SIZE_OF_PACKET_BUFFER,
                           URB_FUNCTION_VENDOR_ENDPOINT,
                           VENDOR_WRITERQST_RAM,
                           ramOffset,
                           ramData,
                           FALSE,
                           pIrp,
                           (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetData);

    return status;
}

///////////////////////////////////////////////////////////////////
//  ReadPortCheckParameters - validates parameters for the user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ReadPortCheckParameters(PIRP                pIrp,
                                            PIO_STACK_LOCATION  pIrpStack)
{
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 
        sizeof(USHORT)) {
        KdPrint(("THERMUSB: CThermUsb::ReadPortCheckParameters: Input Buffer Length was incorrect %d != sizeof(USHORT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength !=
        sizeof(UCHAR)) {
        KdPrint(("THERMUSB: CThermUsb::ReadPortCheckParameters: Output Buffer Length was incorrect %d != sizeof(UCHAR). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }

    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlReadPort - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlReadPort(PIRP                pIrp,
                                          PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS   status;         // status return
    PUCHAR     pBuffer;        // pointer to user buffer
    USHORT     portNumber;     // port number to read

  
    pIrp->IoStatus.Information = 0;   
    pBuffer    = (PUCHAR)(pIrp->AssociatedIrp.SystemBuffer);
    portNumber = (USHORT)(*pBuffer); 

        // The port is read by performing a read request to a PORT
        // using the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_READRQST_PORT 
        // The VALUE field is set to PORT Number passed by the user 
        // The INDEX Field is not used, set to 0.
    status = AsynchronousClassOrVendorRequest(
                           &m_PacketBuffer,
                           SIZE_OF_PACKET_BUFFER,
                           URB_FUNCTION_VENDOR_ENDPOINT,
                           VENDOR_READRQST_PORT,
                           portNumber,
                           0,
                           FALSE,
                           pIrp,
                           (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetData);
    return status;
}

///////////////////////////////////////////////////////////////////
//  WritePortCheckParameters - validates parameters for the user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::WritePortCheckParameters(PIRP                pIrp,
                                             PIO_STACK_LOCATION  pIrpStack)
{ 
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 
        sizeof(PORT_WRITE_STRUCT)) {
        KdPrint(("THERMUSB: CThermUsb::WritePortCheckParameters: Input Buffer Length was incorrect %d != sizeof(PORT_WRITE_STRUCT). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength != 0) {
        KdPrint(("THERMUSB: CThermUsb::WritePortCheckParameters: Output Buffer Length was incorrect %d != 0. returning STATUS_INVALID_PARAMETER.\n",
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }

    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlWritePort - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlWritePort(PIRP                pIrp,
                                           PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS               status;         // status return
    PPORT_WRITE_STRUCT     pBuffer;        // pointer to user buffer
    USHORT                 portNumber;     // port to write
    USHORT                 portData;       // port data

    pIrp->IoStatus.Information = 0;   
    pBuffer    = (PPORT_WRITE_STRUCT)(pIrp->AssociatedIrp.SystemBuffer);
    portNumber = pBuffer->Port;
    portData   = pBuffer->Data;

        // The port is written by performing a write request to PORT
        // using the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_WRITERQST_PORT 
        // The VALUE field is set to the PORT number passed by the user 
        // The INDEX field contains the Data to write
    status = AsynchronousClassOrVendorRequest(
                           &m_PacketBuffer,
                           SIZE_OF_PACKET_BUFFER,
                           URB_FUNCTION_VENDOR_ENDPOINT,
                           VENDOR_WRITERQST_PORT,
                           portNumber,
                           portData,
                           FALSE,
                           pIrp,
                           (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetData);
    return status;

}

///////////////////////////////////////////////////////////////////
//  GetButtonStateCheckParameters - validates parameters for the 
//                                  user request
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: return - NT status code
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::GetButtonStateCheckParameters(PIRP                pIrp,
                                                  PIO_STACK_LOCATION  pIrpStack)
{
        // check the buffer sizes from the user
    if (pIrpStack->Parameters.DeviceIoControl.InputBufferLength != 0) {
        KdPrint(("THERMUSB: CThermUsb::GetButtonStateCheckParameters: Input Buffer Length was incorrect %d != 0. returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.InputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    if (pIrpStack->Parameters.DeviceIoControl.OutputBufferLength !=
        sizeof(UCHAR)) {
        KdPrint(("THERMUSB: CThermUsb::GetButtonStateCheckParameters: Output Buffer Length was incorrect %d != sizeof(UCHAR). returning STATUS_INVALID_PARAMETER.\n", 
            pIrpStack->Parameters.DeviceIoControl.OutputBufferLength));
        return STATUS_INVALID_PARAMETER;
    }
    return STATUS_PENDING;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlGetButtonState - performs request from StartIo
//  Input:  pIrp - IRP request
//          pIrpStack - IRP stack
//  Output: 
//  Notes:  supports METHOD_BUFFERED ioctls only
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbIoctlGetButtonState(PIRP                pIrp,
                                                PIO_STACK_LOCATION  pIrpStack)
{
    NTSTATUS    status;     // status return

    pIrp->IoStatus.Information = 0;   

        // copy our parameters to the parameters for the lower-level
        // USBD driver
    IoCopyCurrentIrpStackLocationToNext(pIrp);

        // The button state is read by performing a read request to RAM
        // using the Vendor request to endpoint funtion
        // The REQUEST field is set to VENDOR_READRQST_RAM 
        // The VALUE field is set to RAM offset denoted by BUTTON_STATE_OFFSET  
        // The INDEX Field is not used, set to 0.
    status = AsynchronousClassOrVendorRequest(
                            &m_PacketBuffer,
                            SIZE_OF_PACKET_BUFFER,
                            URB_FUNCTION_VENDOR_ENDPOINT,
                            VENDOR_READRQST_RAM,
                            BUTTON_STATE_OFFSET,
                            0,
                            FALSE,
                            pIrp,
                            (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetData);    

    return status;
}

//////////////////////////////////////////////////////////////////
//  ThermUsbCompletionGetData - completion routine for USB 
//                              asynchronous request 
//                              Gets user data out of buffer 
//                              if necessary.
//  Input:  pDeviceObject - pointer to device object
//          pIrp - IRP request
//          pContext - pointer to the context
//  Output: return - NT status code
//  Notes:  
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbCompletionGetData(PDEVICE_OBJECT  pDeviceObject, 
                                              PIRP            pIrp,
                                              PVOID           pContext)
{
    PIO_STACK_LOCATION  pIrpStack;      // the current irp stack
    CThermUsb           *pDevice;       // our device object
    PVOID               pBuffer;        // pointer to user buffer                
    PWINDK_URB_CONTEXT  pUrbContext 
                        = (PWINDK_URB_CONTEXT)pContext; // URB Context


        // make sure we did not get a NULL context
    WINDK_ASSERT(pContext != NULL);
         // propagate the pending flag
    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }
        // get the current irp stack location
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
        // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, 
                                        pUrbContext->pDeviceObject);

        // if IRP was cancelled then we decrement IRP and return
    if (pDevice->CheckIrpCanceled(pIrp)) {
        pDevice->FreeUrbContext(pUrbContext);
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pDevice->IrpCountDecrement();    
        return STATUS_CANCELLED;
    }

    if (NT_SUCCESS(pIrp->IoStatus.Status)) {
            // Figure out which IOCTL we are handling 
        switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode) {

            case IOCTL_THERMUSB_GET_BUTTON_STATE:
                pBuffer  = pIrp->AssociatedIrp.SystemBuffer; 
                *((PUCHAR)pBuffer) = pDevice->m_PacketBuffer[1]; 
                pIrp->IoStatus.Information = sizeof(UCHAR);             
                break;

            case IOCTL_THERMUSB_READ_RAM:
                pBuffer  = pIrp->AssociatedIrp.SystemBuffer; 
                *((PUCHAR)pBuffer) = pDevice->m_PacketBuffer[1];
                pIrp->IoStatus.Information = sizeof(UCHAR);  
                break;

            case IOCTL_THERMUSB_READ_PORT:
                pBuffer  = pIrp->AssociatedIrp.SystemBuffer; 
                *((PUCHAR)pBuffer) = pDevice->m_PacketBuffer[1];   
                pIrp->IoStatus.Information = sizeof(UCHAR);  
                break;

            case IOCTL_THERMUSB_WRITE_RAM:
                    // Nothing to do
                break;

            case IOCTL_THERMUSB_WRITE_PORT:
                    // Nothing to do
                break;
        }
    }
        // Free the Urb Context
    pDevice->FreeUrbContext(pUrbContext);
        // Start next
    pDevice->StartNextPacket();
    pDevice->IrpCountDecrement();

    return pIrp->IoStatus.Status;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbCompletionGetTempLow - completion routine for USB 
//                                 asynchronous request for
//                                 temperature (low Byte).  
//                                 Gets user data out of buffer.
//  Input:  pDeviceObject - pointer to device object
//          pIrp - IRP request
//          pContext - pointer to the context
//  Output: return - NT status code
//  Notes:  
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbCompletionGetTempLow(PDEVICE_OBJECT  pDeviceObject, 
                                                 PIRP            pIrp,
                                                 PVOID           pContext)
{
    CThermUsb           *pDevice;       // our device object
    PUSHORT             pBuffer;        // pointer to user buffer                
    PWINDK_URB_CONTEXT  pUrbContext
                        = (PWINDK_URB_CONTEXT)pContext; // URB context
    NTSTATUS            status;         // status return

        // make sure we did not get a NULL context
    WINDK_ASSERT(pContext != NULL);
         // propagate the pending flag
    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }
        // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb,
                                        pUrbContext->pDeviceObject);
        // if IRP was cancelled then we decrement IRP and return
    if (pDevice->CheckIrpCanceled(pIrp)) {
        pDevice->FreeUrbContext(pUrbContext);
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pDevice->IrpCountDecrement();    
        return STATUS_CANCELLED;
    }

        // free the current Urb Context 
    pDevice->FreeUrbContext(pUrbContext);

    if (NT_SUCCESS(pIrp->IoStatus.Status)) {

        pBuffer = (PUSHORT)(pIrp->AssociatedIrp.SystemBuffer);
            // Assemble low byte
        *pBuffer = (USHORT)(pDevice->m_PacketBuffer[1]); 
            // We setup the next Async transfer to get the second portion
            // of data (High byte).

            // copy our parameters to the parameters for the lower-level
            // USBD driver
        IoCopyCurrentIrpStackLocationToNext(pIrp);
            // Read upper bit (9th bit)
            // Everything is the same as getting the low byte
            //    but we are now using the TEMPERATURE_READ_HIGH_OFFSET
        status = pDevice->AsynchronousClassOrVendorRequest(
                            &pDevice->m_PacketBuffer,
                            SIZE_OF_PACKET_BUFFER,
                            URB_FUNCTION_VENDOR_ENDPOINT,
                            VENDOR_READRQST_RAM,
                            TEMPERATURE_READ_HIGH_OFFSET,
                            0,
                            FALSE,
                            pIrp,
                            (PIO_COMPLETION_ROUTINE)ThermUsbCompletionGetTempHigh); 

        if (!NT_SUCCESS(status)) {
                // The request failed for some reason

            KdPrint(("THERMUSB: ThermUsbCompletionGetTempLow req failed 0x%x, %d\n",
                status, pDevice->GetCurrentIrpCount() ));

                // Start next 
            pDevice->StartNextPacket();
            pDevice->IrpCountDecrement();
            return status;
        }  


        return STATUS_MORE_PROCESSING_REQUIRED;

    } else {
            // Lower Driver failed

            // Start next 
        pDevice->StartNextPacket();
        pDevice->IrpCountDecrement();
        KdPrint(("THERMUSB: CompletionGetTempLow lower driver failed 0x%x, %d\n",
           pIrp->IoStatus.Status, pDevice->GetCurrentIrpCount() ));

        return pIrp->IoStatus.Status;
    }
   
}

///////////////////////////////////////////////////////////////////
//  ThermUsbCompletionGetTempHigh - completion routine for USB 
//                                  asynchronous request for
//                                  temperature (high Byte).  
//                                  Gets user data out of buffer.
//  Input:  pDeviceObject - pointer to device object
//          pIrp - IRP request
//          pContext - pointer to the context
//  Output: return - NT status code
//  Notes:  
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbCompletionGetTempHigh(PDEVICE_OBJECT  pDeviceObject, 
                                                  PIRP            pIrp,
                                                  PVOID           pContext)
{
    CThermUsb           *pDevice;       // our device object
    PUSHORT             pBuffer;        // pointer to user buffer                
    PWINDK_URB_CONTEXT  pUrbContext 
                        = (PWINDK_URB_CONTEXT)pContext; // URB context

        // make sure we did not get a NULL context
    WINDK_ASSERT(pContext != NULL);
         // propagate the pending flag
    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }
        // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, 
                                        pUrbContext->pDeviceObject);
        // if IRP was cancelled then we decrement IRP and return
    if (pDevice->CheckIrpCanceled(pIrp)) {
        pDevice->FreeUrbContext(pUrbContext);
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pDevice->IrpCountDecrement();    
        return STATUS_CANCELLED;
    }

 
    if (NT_SUCCESS(pIrp->IoStatus.Status)) {
        pBuffer  = (PUSHORT)(pIrp->AssociatedIrp.SystemBuffer);    
            // Assemble high byte
        *pBuffer = (*pBuffer | 
                (((USHORT)(pDevice->m_PacketBuffer[1])) << 8));  
        pIrp->IoStatus.Information = sizeof(USHORT);  
    }

        // Free the Urb Context 
    pDevice->FreeUrbContext(pUrbContext);
        // Start next 
    pDevice->StartNextPacket();
    pDevice->IrpCountDecrement();            
       
    return pIrp->IoStatus.Status;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbCompletionSetBrightness - completion routine for USB 
//                                    asynchronous requests for
//                                    setting brightness.
//  Input:  pDeviceObject - pointer to device object
//          pIrp - IRP request
//          pContext - pointer to the context
//  Output: return - NT status code
//  Notes:  
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbCompletionSetBrightness(PDEVICE_OBJECT  pDeviceObject, 
                                                    PIRP            pIrp,
                                                    PVOID           pContext)
{
    CThermUsb           *pDevice;       // our device object
    PWINDK_URB_CONTEXT  pUrbContext 
                        = (PWINDK_URB_CONTEXT)pContext; // URB context
    NTSTATUS            status;         // status return

        // make sure we did not get a NULL context
    WINDK_ASSERT(pContext != NULL);
        // propagate the pending flag
    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }
       // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, 
                                        pUrbContext->pDeviceObject);
        // free the Urb Context 
    pDevice->FreeUrbContext(pUrbContext);     

        // if IRP was cancelled then we decrement IRP and return
    if (pDevice->CheckIrpCanceled(pIrp)) {
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pDevice->IrpCountDecrement();    
        return STATUS_CANCELLED;
    }


    if (NT_SUCCESS(pIrp->IoStatus.Status)) {
            // perform the last asynch transfer to trigger the semaphore
        IoCopyCurrentIrpStackLocationToNext(pIrp);
            // The brightness control is set by writing a non-zero value to a 
            //    semaphore with a Write request to RAM.
            // The REQUEST field is set to VENDOR_WRITERQST_RAM 
            // The VALUE field is set to RAM offset denoted by 
            // LED_BRIGHTNESS_SEMAPHORE_OFFSET
            // The INDEX Field contains the Data to be written, 
            //    0xFF in this case.
        status = pDevice->AsynchronousClassOrVendorRequest(
                            &pDevice->m_PacketBuffer,
                            SIZE_OF_PACKET_BUFFER,
                            URB_FUNCTION_VENDOR_ENDPOINT,
                            VENDOR_WRITERQST_RAM,
                            LED_BRIGHTNESS_SEMAPHORE_OFFSET,
                            0xFF,
                            FALSE,
                            pIrp,
                            (PIO_COMPLETION_ROUTINE)ThermUsbCompletionSetLedSemaphore); 

        if (!NT_SUCCESS(status)) {
                // the request failed for some reason
                // Start next 
            pDevice->StartNextPacket();
            pDevice->IrpCountDecrement();
            return status;
        }
        return STATUS_MORE_PROCESSING_REQUIRED;
    } else {
            // Lower driver failed. 
            // Start next 
        pDevice->StartNextPacket();
        pDevice->IrpCountDecrement();
        return pIrp->IoStatus.Status;
    }
}

///////////////////////////////////////////////////////////////////
//  ThermUsbCompletionSetLedSemaphore - completion routine for 
//                                      USB asynchronous requests
//                                      for triggering a brightness 
//                                      change.
//  Input:  pDeviceObject - pointer to device object
//          pIrp - IRP request
//          pContext - pointer to the context
//  Output: return - NT status code
//  Notes:  
///////////////////////////////////////////////////////////////////
NTSTATUS CThermUsb::ThermUsbCompletionSetLedSemaphore(PDEVICE_OBJECT  pDeviceObject, 
                                                      PIRP            pIrp,
                                                      PVOID           pContext)
{
    CThermUsb           *pDevice;       // our device object
    PWINDK_URB_CONTEXT  pUrbContext 
                        = (PWINDK_URB_CONTEXT)pContext; // URB context

        // make sure we did not get a NULL context
    WINDK_ASSERT(pContext != NULL);
         // propagate the pending flag
    if (pIrp->PendingReturned) {
        IoMarkIrpPending(pIrp);
    }
        // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, 
                                        pUrbContext->pDeviceObject);     
        // free the Urb Context 
    pDevice->FreeUrbContext(pUrbContext);
        // if IRP was cancelled then we decrement IRP and return
    if (pDevice->CheckIrpCanceled(pIrp)) {
        pIrp->IoStatus.Status = STATUS_CANCELLED;
        pDevice->IrpCountDecrement();    
        return STATUS_CANCELLED;
    }
        // start next 
    pDevice->StartNextPacket();
    pDevice->IrpCountDecrement();
    return pIrp->IoStatus.Status;
}
