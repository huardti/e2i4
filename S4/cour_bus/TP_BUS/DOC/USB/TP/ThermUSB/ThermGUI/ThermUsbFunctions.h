//////////////////////////////////////////////////////////////////////////////
//  ThermUsbFunctions.h - IOCTL caller functions nad helpers for ThermUSB
//  Copyright (c) 1998, BlueWater Systems, Inc.
//////////////////////////////////////////////////////////////////////////////

    // set the brightness of the LED (value 0x0 - 0xf)
BOOLEAN SetLedBrightness(HANDLE hThermUsb, USHORT Brightness);
    // get the button state on the development board
LONG    GetButtonState(HANDLE hThermUsb); 
    // get raw temperature 
BOOLEAN GetRawTemperature(HANDLE hThermUsb, PUSHORT pRawTemp);
    // convert raw temperature to degrees C
float   GetTempCFromRawTemp(USHORT RawTemp);
    // convert raw temperature to degrees F
float   GetTempFFromRawTemp(USHORT RawTemp);
