/////////////////////////////////////////////////////////////////
//  TTest.cpp - user mode test program main routine
//  Copyright (c) 1998, BlueWater Systems, Inc.
/////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winioctl.h>
#include <process.h>
#include <memory.h>
#include "DDThermU.h"

    // prototypes
LONG MainHelp(int argc, char *argv[], PVOID pInterface);
LONG ThermUsbDispatch(int argc, char *argv[], PVOID pInterface);

struct _COMMAND_TABLE
{
    PCHAR   Command;
    LONG    (*DispatchFunction)(int argc, char *argv[], PVOID pInterface);
    PVOID   pInterface;
} Commands[] =
{   // Command          Function                    Symbolic Link Name or Interface GUID
    "-?",               MainHelp,                   NULL,
    "/?",               MainHelp,                   NULL,
    "-h",               MainHelp,                   NULL,
    "-d0",              ThermUsbDispatch,           "\\\\.\\ThermUsb0",
    "-d1",              ThermUsbDispatch,           "\\\\.\\ThermUsb1",
};

#define NUMBER_OF_COMMANDS\
    sizeof(Commands)/sizeof(struct _COMMAND_TABLE)

void __cdecl main(int argc, char *argv[])
{
    LONG    ii;             // loop variable
    LONG    status;         // return status from command

    if (argc < 2) {
        MainHelp(argc, argv, NULL);
        exit(-1);
    }

        // find the entered command
    for(ii = 0; ii < NUMBER_OF_COMMANDS; ii++) {
        if (lstrcmpi(argv[1], Commands[ii].Command) == 0) {
                // dispatch to the correct handler
            status = (Commands[ii].DispatchFunction)(argc, argv, Commands[ii].pInterface);
            exit(status);
        }
    }
        // command not found
    MainHelp(argc, argv, NULL);
    exit(-1);
}

LONG MainHelp(int argc, char *argv[], PVOID pInterfaceName)
{
    printf("ThermUsb Test\n"
           "\t-?, /?, -h                    - Help message\n"
           "\t-d0 Commands                  - Send command to ThermUsb0\n"
           "\t\tIn order to obtain help for this device, use -d0 -?\n"
           );
    return TRUE;
}
