/////////////////////////////////////////////////////////////////
//  ThermUsbTest.cpp - user interface functions
//  Copyright (c) 1998, BlueWater Systems, Inc.
/////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <winioctl.h>
#include <process.h>
#include <memory.h>
#include "DDThermU.h"

    // the amount of RAM that we read using IOCTL_THERMUSB_READ_RAM
#define RAM_SIZE 128

    // prototypes
LONG ThermUsbHelp(int argc, char *argv[]);
LONG ThermUsbTestSetLedBrightness(int argc, char *argv[]);
LONG ThermUsbTestGetTemperature(int argc, char *argv[]);
LONG ThermUsbTestReadRam(int argc, char *argv[]);
LONG ThermUsbTestWriteRam(int argc, char *argv[]);
LONG ThermUsbTestReadPort(int argc, char *argv[]);
LONG ThermUsbTestWritePort(int argc, char *argv[]);

struct _THERMUSB_COMMAND_TABLE
{
    PCHAR   Command;
    LONG    (*DispatchFunction)(int argc, char *argv[]);
} Commands[] =
{   // Command      Function
    "-?",           ThermUsbHelp,
    "/?",           ThermUsbHelp,
    "-h",           ThermUsbHelp,
    "-i0",          ThermUsbTestSetLedBrightness,
    "-i1",          ThermUsbTestGetTemperature,
    "-i2",          ThermUsbTestReadRam,
    "-i3",          ThermUsbTestWriteRam,
    "-i4",          ThermUsbTestReadPort,
    "-i5",          ThermUsbTestWritePort,
};

#define NUMBER_OF_COMMANDS\
    sizeof(Commands)/sizeof(struct _THERMUSB_COMMAND_TABLE)

    // device handle
HANDLE hThermUsb = INVALID_HANDLE_VALUE;

LONG ThermUsbDispatch(int argc, char *argv[], PVOID pInterface)
{
    LONG    ii;             // loop variable
    LONG    status;         // return status from command

    if (argc < 3) {
        ThermUsbHelp(argc, argv);
        return -1;
    }

        // find the entered command
    for(ii = 0; ii < NUMBER_OF_COMMANDS; ii++) {
        if (lstrcmpi(argv[2], Commands[ii].Command) == 0) {
                // open a handle to the device
            hThermUsb = CreateFile(
                        (PCHAR)pInterface,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
            if (hThermUsb == INVALID_HANDLE_VALUE) {
                printf("**Error:  couldn't open %s, (%d)\n", pInterface, GetLastError());
                return -1;
            }
                // dispatch command
            status = (Commands[ii].DispatchFunction)(argc, argv);
            CloseHandle(hThermUsb);
            return status;
        }
    }
        // command not found
    ThermUsbHelp(argc, argv);
    return -1;
}

LONG ThermUsbHelp(int argc, char *argv[])
{
    printf("ThermUsb Test\n"
           "\t-?, /?, -h         - Help message\n"
           "\t-i0                - Test SetLedBrightness\n"
           "\t-i1                - Test GetTemperature\n"
           "\t-i2                - Test ReadRam\n"
           "\t-i3                - Test WriteRam\n"
           "\t-i4                - Test ReadPort\n"
           "\t-i5                - Test WritePort\n"
           );
    return TRUE;
}

VOID ConvertBytetoBinary(PCHAR pBinaryString, UCHAR Data)
{
    LONG  count;         // loop variable
    PCHAR pBitPosition = pBinaryString; // pointer to the bit position
    UCHAR mask = 0x80;  // mask value

    for (count = 0 ; count < 8; count++) {
        if ((mask & Data) != 0) {
            *pBitPosition  = '1';
        } else {
            *pBitPosition  = '0';
        }
        mask = mask >> 1;
        pBitPosition++;
    }
}

LONG ThermUsbTestSetLedBrightness(int argc, char *argv[])
{
    ULONG   length;         // the return length from the driver
    USHORT  brightness = 0; // the brightness
    UCHAR   buttonState;    // the state of the button
    DWORD   brightnessInterval = 500;  // In milliseconds

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    printf("IOCTL_THMUSB_SET_LED_BRIGHTNESS. Press any key to exit. \n");
    printf("Press Momentary Switch to change blink rate \n");

    for (;;) {

        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_SET_LED_BRIGHTNESS,
            &brightness,
            sizeof(USHORT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_SET_LED_BRIGHTNESS failed:  0x%X\n", GetLastError());
            return FALSE;
        }
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_GET_BUTTON_STATE,
            NULL,
            0,
            &buttonState,
            sizeof(UCHAR),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_GET_BUTTON_STATE failed:  0x%X\n", GetLastError());
            return FALSE;
        }


        if (buttonState) {
            brightnessInterval = 100;
        } else {
            brightnessInterval = 500;
        }

        printf("Button State : %d\r", buttonState);
        brightness = ~brightness;
        Sleep(brightnessInterval);  // Delay

        if (_kbhit()) {
                // user wants to exit
            printf("\nExit\n");
            return TRUE;
        }
    }

    return TRUE;
}

LONG ThermUsbTestGetTemperature(int argc, char *argv[])
{
    ULONG   length;         // the return length from the driver
    USHORT  rawTemp;        // temperature returned from the device
    UCHAR   buttonState;    // the state of the button
    float   temperatureConversionF,temperatureConversionC;  // human-readable temperature

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    printf("IOCTL_THERMUSB_GET_TEMPERATURE Test:  \n");
    printf("Press momentary switch to change English or Metric display. \n");
    printf("Press any key to exit. \n\n");

    for (;;) {

        rawTemp = 0;
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_GET_TEMPERATURE,
            NULL,
            0,
            &rawTemp,
            sizeof(USHORT),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_GET_TEMPERATURE failed:  %d\n", GetLastError());
            return FALSE;
        }

        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_GET_BUTTON_STATE,
            NULL,
            0,
            &buttonState,
            sizeof(UCHAR),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_GET_BUTTON_STATE failed:  %d\n", GetLastError());
            return FALSE;
        }

        if (rawTemp >= 0x100) {
            rawTemp = 0x200 - rawTemp;
            temperatureConversionF = (float)(-((((float)rawTemp * 0.5)* 9/5) + 32));
            temperatureConversionC = (float)(-((float)rawTemp * 0.5));
        } else {
            temperatureConversionF = (float)((((float)rawTemp * 0.5)* 9/5) + 32);
            temperatureConversionC = (float)((float)rawTemp * 0.5);
        }

        if (buttonState) {
            printf("Temperature(F) : %3.1f \r", temperatureConversionF);
        } else {
            printf("Temperature(C) : %3.1f \r", temperatureConversionC);
        }

        Sleep(500);  // wait 500 ms

            // check for user exit
        if (_kbhit()) {
                // user wants to exit
            printf("\nExit\n");
            return TRUE;
        }
    }

    return TRUE;
}

LONG ThermUsbTestReadRam(int argc, char *argv[])
{
    ULONG   length;         // the return length from the driver
    USHORT  address;        // address of data to read from RAM
    UCHAR   ramData[255];   // data from RAM
    UCHAR   returnData;     // data read from RAM
    USHORT  lineCount = 0;  // linecount for formatting display

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    printf("IOCTL_THERMUSB_READ_RAM. Ram Dump \n\n");
    printf("Address                  Data \n\n");

    for (address = 0; address < RAM_SIZE; address++) {
        returnData = 0;
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_READ_RAM,
            &address,
            sizeof(USHORT),
            &returnData,
            sizeof(UCHAR),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_READ_RAM failed:  %d\n", GetLastError());
            return FALSE;
        } else {
            ramData[address] = (UCHAR)returnData;
        }

    }

    for (address = 0; address < RAM_SIZE; address++) {
        if (lineCount == 0) {
            printf("0x%2.2X  ", address);
        }

        printf(" %2.2X", ramData[address]);
        lineCount++;
        if (lineCount == 16){
            lineCount = 0;
            printf("\n");
        }
    }

    return TRUE;
}

LONG ThermUsbTestWriteRam(int argc, char *argv[])
{
    ULONG            length;     // the return length from the driver
    RAM_WRITE_STRUCT ramInfo;    // RAM information

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    printf("IOCTL_THERMUSB_WRITE_RAM. Press any key to exit. \n\n");
    printf("LED should be blinking..... \n");

    for (;;) {

        ramInfo.Address = 0x2C; // address for LED brightness
        ramInfo.Data    = 0x00; // set RAM location to 0
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_RAM,
            &ramInfo,
            sizeof(RAM_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_RAM failed:  %d\n", GetLastError());
            return FALSE;
        }
        ramInfo.Address = 0x2B; // address for LED brightness update semaphore
        ramInfo.Data    = 0xFF; // set RAM location to FF
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_RAM,
            &ramInfo,
            sizeof(RAM_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_RAM failed:  %d\n", GetLastError());
            return FALSE;
        }

        Sleep(200);

        ramInfo.Address = 0x2C; // address for LED brightness
        ramInfo.Data    = 0xFF; // set RAM location to FF
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_RAM,
            &ramInfo,
            sizeof(RAM_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_RAM failed:  %d\n", GetLastError());
            return FALSE;
        }
        ramInfo.Address = 0x2B; // address for LED brightness update semaphore
        ramInfo.Data    = 0xFF; // set RAM location to FF
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_RAM,
            &ramInfo,
            sizeof(RAM_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_RAM failed:  %d\n", GetLastError());
            return FALSE;
        }

        Sleep(200);

        if (_kbhit()) {
                // user wants to exit
            printf("\nExit\n");
            return TRUE;
        }
    }

    return TRUE;
}

LONG ThermUsbTestReadPort(int argc, char *argv[])
{
    ULONG   length;             // the return length from the driver
    CHAR    port0val[9];        // formatted data from port 0
    CHAR    port1val[9];        // formatted data from port 1
    USHORT  portNumber = 0;     // port number to read
    UCHAR   portData = 0;       // data read from port

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    port0val[8] = NULL;
    port1val[8] = NULL;

    printf("IOCTL_THERMUSB_READ_PORT. Press any key to exit. \n\n");
    printf("Port #: MSB.....LSB \n");

    for (;;) {

        portNumber = 0;
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_READ_PORT,
            &portNumber,
            sizeof(USHORT),
            &portData,
            sizeof(UCHAR),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_READ_PORT failed:  %d\n", GetLastError());
            return FALSE;
        }
        ConvertBytetoBinary(&port0val[0], portData);

        portNumber = 1;
        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_READ_PORT,
            &portNumber,
            sizeof(USHORT),
            &portData,
            sizeof(UCHAR),
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_READ_PORT failed:  %d\n", GetLastError());
            return FALSE;
        }
        ConvertBytetoBinary(&port1val[0], portData);

        printf("Port 0:%s Port 1:%s \r",port0val, port1val);
        Sleep(100);
        if (_kbhit()) {
                // user wants to exit
            printf("\nExit\n");
            return TRUE;
        }
    }

    return TRUE;
}

LONG ThermUsbTestWritePort(int argc, char *argv[])
{
    ULONG             length;   // the return length from the driver
    PORT_WRITE_STRUCT portInfo; // port information

    if (argc < 3) {
            // too few parameters
        ThermUsbHelp(argc, argv);
        return FALSE;
    }

    printf("IOCTL_THERMUSB_WRITE_PORT. Press any key to exit. \n\n");
    printf("Ports P10, P11 should be toggling. **Must have revised firmware** \n");

    for (;;) {
        portInfo.Port = 1;    // use port 1
        portInfo.Data = 0x04; // set all bits to 0 except bit 2(switch input)

        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_PORT,
            &portInfo,
            sizeof(PORT_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_PORT failed:  %d\n", GetLastError());
            return FALSE;
        }

        Sleep(500);          // delay

        portInfo.Port = 1;    // use port 1
        portInfo.Data = 0x0F; // set all bits to 1.

        if (!DeviceIoControl(
            hThermUsb,
            IOCTL_THERMUSB_WRITE_PORT,
            &portInfo,
            sizeof(PORT_WRITE_STRUCT),
            NULL,
            0,
            &length,
            NULL)) {
            printf("IOCTL_THERMUSB_WRITE_PORT failed:  %d\n", GetLastError());
            return FALSE;
        }

        if (_kbhit()) {
                // user wants to exit
            printf("\nExit\n");
            return TRUE;
        }

    }

    return TRUE;
}
