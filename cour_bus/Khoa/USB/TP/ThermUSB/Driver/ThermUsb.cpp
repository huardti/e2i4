/////////////////////////////////////////////////////////////////
//  ThermUsb.cpp - CThermUsb implementation
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

/////////////////////////////////////////////////////////////////////
//  ~CThermUsb Destructor
//  Input:  
//  Output: 
//  Notes:  The destructor must be defined before the constructor so
//          that compiler generated destructors will be placed in
//          the correct code segment.
/////////////////////////////////////////////////////////////////////
CThermUsb::~CThermUsb()
{
}

#ifdef WINDK_CODE_SEG_PRAGMA
#pragma code_seg("PAGE")
#endif //WINDK_CODE_SEG_PRAGMA

/////////////////////////////////////////////////////////////////////
//  CThermUsb - Constructor for the CThermUsb class
//  Input:  pDriverObject   - driver object
//          pPhysicalDeviceObject - physical device object
//          pDeviceName     - name of this device
//          DeviceType      - device's type
//          pRegistryPath   - path to the registry entries for this driver
//          DeviceNumber    - device number
//          pContext        - not used
//  Output: ppDeviceObject  - NT device object for this instance of the device
//  Notes:  throws exception on error
//////////////////////////////////////////////////////////////////////
CThermUsb::CThermUsb(      PDRIVER_OBJECT    pDriverObject,
                     CONST PDEVICE_OBJECT    pPhysicalDeviceObject,
                     CONST PWCHAR            pDeviceName,
                     CONST ULONG             DeviceType,
                           PUNICODE_STRING   pRegistryPath,
                     CONST ULONG             DeviceNumber,
                           PDEVICE_OBJECT    *ppDeviceObject,
                           PVOID             pContext) :
    CUsb(pDriverObject,
         pPhysicalDeviceObject,
         pDeviceName,
         DeviceType,
         ppDeviceObject)
{
    NTSTATUS            status;                 // status return

        // create a Win32 accessable name for the driver 
        // in the form of ThermUsb0
    status = CreateWin32Name(Concatenate(L"ThermUsb", DeviceNumber));
    if (!NT_SUCCESS(status)) {
        ThrowStatusExceptionFromConstructor(status, 
            "THERMUSB:CThermUsb::CThermUsb CreateWin32Name failed");
    }   
}

#ifdef WINDK_CODE_SEG_PRAGMA
#pragma code_seg( )
#endif //WINDK_CODE_SEG_PRAGMA

