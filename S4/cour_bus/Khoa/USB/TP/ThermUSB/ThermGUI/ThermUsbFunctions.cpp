//////////////////////////////////////////////////////////////////////////////
//  ThermUsbFunctions.cpp - IOCTL caller functions nad helpers for ThermUSB
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <winioctl.h>
#include "ThermUsbFunctions.h"
#include "..\inc\DDThermU.h"

//////////////////////////////////////////////////////////////////////////////
// SetLedBrightness - sets the brightness of the green LED
// Inputs:  hThermUsb - handle to ThermUsb device
//          Brightness - brightness from 0x00 (off) to 0xf (fully on)
// Output:
// Notes:   16 positions (0 through 0xf)
//////////////////////////////////////////////////////////////////////////////
BOOLEAN SetLedBrightness(HANDLE hThermUsb, USHORT Brightness)
{
    ULONG   length;                   // return length from the driver

    if (!DeviceIoControl(
        hThermUsb, 
        IOCTL_THERMUSB_SET_LED_BRIGHTNESS,
        &Brightness,
        sizeof(USHORT),
        NULL,
        0,
        &length,
        NULL)) {

// TODO: Add the following code to display a message box on error
//            CHAR strError[256];
//            sprintf(strError, "IOCTL_THERMUSB_SET_LED_BRIGHTNESS\nfailed with error %d", GetLastError());
//            AfxMessageBox(strError);
            return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// GetButtonState - gets the Cypress board button state
// Inputs:  hThermUsb - handle to ThermUsb device
// Output:
// Notes:   returns 0 for one button state, 1 for the other, -1 for an error
//////////////////////////////////////////////////////////////////////////////
LONG GetButtonState(HANDLE hThermUsb) 
{
    ULONG   length;         // the return length from the driver
    UCHAR   buttonState;    // the state of the button

     if (!DeviceIoControl(
            hThermUsb, 
            IOCTL_THERMUSB_GET_BUTTON_STATE,
            NULL,
            0,
            &buttonState,
            sizeof(UCHAR),
            &length,
            NULL)) {

// TODO: Add the following code to display a message box on error
//            CHAR strError[256];
//            sprintf(strError, "IOCTL_THERMUSB_GET_BUTTON_STATE\nfailed with error %d", GetLastError());
//            AfxMessageBox(strError);
            return -1;
     }

     if (buttonState) {
        return 1;
     } else {
        return 0;
     }
}

//////////////////////////////////////////////////////////////////////////////
// GetRawTemperature - gets raw temperature
// Inputs:  hThermUsb - handle to ThermUsb device
// Output:  pRawTemp - pointer to raw temperature
// Notes:   
//////////////////////////////////////////////////////////////////////////////
BOOLEAN GetRawTemperature(HANDLE hThermUsb, PUSHORT pRawTemp)
{
    ULONG   length;         // the return length from the driver

    if (!DeviceIoControl(
        hThermUsb, 
        IOCTL_THERMUSB_GET_TEMPERATURE,
        NULL,
        0,
        pRawTemp,
        sizeof(USHORT),
        &length,
        NULL)) {

// TODO: Add the following code to display a message box on error
//            CHAR strError[256];
//            sprintf(strError, "IOCTL_THERMUSB_GET_TEMPERATURE\nfailed with error %d", GetLastError());
//            AfxMessageBox(strError);
            *pRawTemp = 0;
            return FALSE;   
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// GetTempCFromRawTemp - gets degrees C
// Inputs:  RawTemp - raw temperature value
// Output:  
// Notes:   
//////////////////////////////////////////////////////////////////////////////
float GetTempCFromRawTemp(USHORT RawTemp)
{
    if (RawTemp >= 0x100) {
        RawTemp = 0x200 - RawTemp;
        return (float)(-((float)RawTemp * 0.5));
    } else {
        return (float)((float)RawTemp * 0.5); 
    }
}

//////////////////////////////////////////////////////////////////////////////
// GetTempFFromRawTemp - gets degrees F
// Inputs:  RawTemp - raw temperature value
// Output:  
// Notes:   
//////////////////////////////////////////////////////////////////////////////
float GetTempFFromRawTemp(USHORT RawTemp)
{
    if (RawTemp >= 0x100) {
        RawTemp = 0x200 - RawTemp;
        return (float)(-((((float)RawTemp * 0.5)* 9/5) + 32));
    } else {
        return (float)((((float)RawTemp * 0.5)* 9/5) + 32); 
    }
}

