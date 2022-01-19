/////////////////////////////////////////////////////////////////
//  drventry.cpp - ThermUsb driver entry point
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

#ifdef ALLOC_PRAGMA
    // routines which can be discarded after initialization
#pragma alloc_text(INIT,DriverEntry)
    // routines which can be paged out while not in use
#pragma alloc_text(PAGE,ThermUsbAddDevice)
#endif

///////////////////////////////////////////////////////////////////
//  DriverEntry - initial driver entry point for ThermUsb driver
//  Input:  pDriverObject - driver object
//          pRegistryPath - path to this driver's registry entries
//  Output: return() - NTSTATUS value
//  Notes:
///////////////////////////////////////////////////////////////////
NTSTATUS CALLBACK DriverEntry(PDRIVER_OBJECT   pDriverObject,
                              PUNICODE_STRING  pRegistryPath)
{
    CPnpDriver  *pTheDriver = NULL;             // the only driver

    KdPrint(("THERMUSB:  DriverEntry\n"));

        // create the temporary driver class
    pTheDriver = new CPnpDriver(pDriverObject, pRegistryPath);
    if (pTheDriver == NULL) {
            // fatal error, no memory
        KdPrint(("THERMUSB:  couldn't create CPnpDriver\n"));
        return STATUS_NO_MEMORY;
    }

        // setup the callback functions
    pTheDriver->SetCallback((PDRIVER_STARTIO) ThermUsbStartIo);
    pTheDriver->SetCallback(IRP_MJ_DEVICE_CONTROL, 
                    (PDRIVER_DISPATCH) ThermUsbIoctlDispatch);
    pTheDriver->SetCallback(ThermUsbAddDevice);

    delete pTheDriver;

    KdPrint(("THERMUSB: DriverEntry returning Success\n"));
    return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
//  ThermUsbAddDevice - creates a new device object
//  Input:  pDriverObject   - our driver object
//          pPhysicalDeviceObject  - physical device object
//  Output: NTSTATUS code
//  Notes:  Creates device object but doesn't touch resources
///////////////////////////////////////////////////////////////////////////
NTSTATUS CALLBACK ThermUsbAddDevice(PDRIVER_OBJECT pDriverObject,
                                    PDEVICE_OBJECT pPhysicalDeviceObject)
{
    CThermUsb           *pDevice = NULL;    // our device object
    NTSTATUS            status;             // the return status
    PDEVICE_OBJECT      pDeviceObject;      // current NT device object
    ULONG               instance;           // current instance of the device we are creating

        // since PnP can add and delete us in any order, search for an unused
        // device name (instead of just incrementing an instance number)
        // find the first instance number that is not already used
        // (also, a device interface could be used instead of a symbolic link)
    for (instance = 0; instance < MAX_USB_DEVICES; instance++) {

            // create the device name
        CString deviceName = Concatenate(L"ThermUsb", instance);

            // create the device object - this will call the 
            // CThermUsbDevice constructor
        status = CreateDevice (pDriverObject,
                    pPhysicalDeviceObject, 
                    deviceName, 
                    THERMUSB_DEVICE_TYPE, 
                    CPnpDriver::GetDriverRegistryPath(pDriverObject),
                    instance,
                    &pDevice,
                    &pDeviceObject);

        if (NT_SUCCESS(status)) {
                // clear the initializing flag                  
            pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
            KdPrint(("THERMUSB: AddDevice, returning STATUS_SUCCESS\n"));
            return STATUS_SUCCESS;
        } else {
            if (NULL != pDevice) {
                delete pDevice;
            }
                // Create failed, reset pointer.
            pDevice = NULL;
        }

    }

        // failed to create any devices
    KdPrint(("THERMUSB:  Failed to CreateDevice ThermUsb status(0x%X)\n", status));

    WINDK_ASSERT(NT_SUCCESS(status));

    return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbIoctlDispatch - Device I/O control dispatch entry point
//  Input:  pDeviceObject - NT device object
//          pIrp - IRP being dispatched
//  Output: return - NT status value
//  Notes:  handles only IRP_MJ_DEVICE_CONTROL
///////////////////////////////////////////////////////////////////
NTSTATUS CALLBACK ThermUsbIoctlDispatch(PDEVICE_OBJECT pDeviceObject,
                                        PIRP           pIrp)
{
    CThermUsb           *pDevice;       // our device object
    PIO_STACK_LOCATION  pIrpStack;      // our portion of the irp
    NTSTATUS            status;         // return status

         // get our device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, pDeviceObject);
         // make sure it's ok with PnP to take an irp right now 
         // and if so increment irp count
    if (!pDevice->IrpPrologue(pIrp, &status)) {
	    return status;
    }

        // initialize the status for IRPs that we do not accept
    status = STATUS_NOT_IMPLEMENTED;
        // Initialize the irp info field
    pIrp->IoStatus.Information = 0;
        // Get our portion of the irp
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

        // dispatch ioctls
    switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode) {

      case IOCTL_THERMUSB_SET_LED_BRIGHTNESS:
              // check the parameters of the ioctl
          status = pDevice->SetLedBrightnessCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case IOCTL_THERMUSB_GET_TEMPERATURE:
              // check the parameters of the ioctl
          status = pDevice->GetTemperatureCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case IOCTL_THERMUSB_READ_RAM:
              // check the parameters of the ioctl
          status = pDevice->ReadRamCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case IOCTL_THERMUSB_WRITE_RAM:
              // check the parameters of the ioctl
          status = pDevice->WriteRamCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case IOCTL_THERMUSB_READ_PORT:
              // check the parameters of the ioctl
          status = pDevice->ReadPortCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case IOCTL_THERMUSB_WRITE_PORT:
              // check the parameters of the ioctl
          status = pDevice->WritePortCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;

      case  IOCTL_THERMUSB_GET_BUTTON_STATE:
              // check the parameters of the ioctl
          status = pDevice->GetButtonStateCheckParameters(pIrp, pIrpStack);
              // queue the request if the parameters are correct using
              // the default cancel routine
          if (status == STATUS_PENDING) {
              pDevice->QueueIrp(pIrp, WinDKIrpCancelCallback);
          }
          break;
    }

        // complete finished requests
    if (status != STATUS_PENDING) {
        pDevice->CompleteIrp(pIrp, status);
    }

    return status;
}

///////////////////////////////////////////////////////////////////
//  ThermUsbStartIo - ThermUsb StartIo entry point
//  Input:  pDeviceObject - NT device object
//          pIrp - IRP from queue
//  Output:
//  Notes:  handles all queued requests
///////////////////////////////////////////////////////////////////
VOID CALLBACK ThermUsbStartIo(PDEVICE_OBJECT pDeviceObject,
                              PIRP           pIrp)
{
    NTSTATUS            status;     // intermediate status
    PIO_STACK_LOCATION  pIrpStack;  // our portion of the irp
    CThermUsb           *pDevice;   // our device object
    
        // get the device class object
    pDevice = GetDeviceFromDeviceObject(CThermUsb, pDeviceObject);

        // check for already cancelled irp
    if (pDevice->CheckIrpCanceled(pIrp)) {
        return;
    }

        // initialize the irp info field
    pIrp->IoStatus.Information = 0;
        // get our portion of the irp
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
        // dispatch based on major function code
    switch(pIrpStack->MajorFunction) {

      case IRP_MJ_DEVICE_CONTROL:
              // dispatch ioctls
          switch (pIrpStack->Parameters.DeviceIoControl.IoControlCode) {

            case IOCTL_THERMUSB_SET_LED_BRIGHTNESS:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlSetLedBrightness(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_GET_TEMPERATURE:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlGetTemperature(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_READ_RAM:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlReadRam(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_WRITE_RAM:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlWriteRam(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_READ_PORT:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlReadPort(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_WRITE_PORT:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlWritePort(pIrp, pIrpStack);
                break;

            case IOCTL_THERMUSB_GET_BUTTON_STATE:
                    // call the routine to handle the ioctl
                status = pDevice->ThermUsbIoctlGetButtonState(pIrp, pIrpStack);
                break;

          }
          break;
    }

    if (status != STATUS_PENDING) {
            // complete finished requests
        pDevice->CompleteIrpAndStartNext(pIrp, status);
    } 

}

