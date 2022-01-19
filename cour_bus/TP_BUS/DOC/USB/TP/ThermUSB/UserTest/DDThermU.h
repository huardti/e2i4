///////////////////////////////////////////////////////////
//  Thermusb user application interface
//  Copyright (c) 1998, BlueWater Systems, Inc.
///////////////////////////////////////////////////////////

#define THERMUSB_DEVICE_TYPE            0x8000
#define THERMUSB_MAKE_IOCTL(t,c)\
        (ULONG)CTL_CODE((t), 0x800+(c), METHOD_BUFFERED, FILE_ANY_ACCESS)

    //  Input:  USHORT specifying brightness level from 0-255.
    //  Output: none.
#define IOCTL_THERMUSB_SET_LED_BRIGHTNESS\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 0)

    //  Input:  none
    //  Output: USHORT returning temperature value.  Only bits 8-0 are valid.
#define IOCTL_THERMUSB_GET_TEMPERATURE\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 1)
 
    //  Input:  USHORT specify address, valid address: 0-255.
    //  Output: UCHAR returns a byte read from the RAM
#define IOCTL_THERMUSB_READ_RAM\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 2)

    //  Input:  RAM_WRITE_STRUCT specifying address and data
    //  Output: none.
#define IOCTL_THERMUSB_WRITE_RAM\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 3)

    //  Input:  USHORT specifying port number (0 or 1)
    //  Output: UCHAR returns a byte read back from the port
#define IOCTL_THERMUSB_READ_PORT\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 4)

    //  Input:  PORT_WRITE_STRUCT specifying port (0 or 1) and data
    //  Output: none.
#define IOCTL_THERMUSB_WRITE_PORT\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 5)

    //  Input:  none.
    //  Output: UCHAR returns button state (0 or 1) 
#define IOCTL_THERMUSB_GET_BUTTON_STATE\
            THERMUSB_MAKE_IOCTL(THERMUSB_DEVICE_TYPE, 6)

    // Write RAM data structure
typedef struct _RAM_WRITE_STRUCT {
    USHORT  Address;    // Address. Only 0-255 are valid.
    USHORT  Data;       // Data to write  Only 0-255 are valid
} RAM_WRITE_STRUCT, *PRAM_WRITE_STRUCT;

    // Write Port data structure
typedef struct _PORT_WRITE_STRUCT {
    USHORT  Port;       // Port to write, 0 or 1.
    USHORT  Data;       // Data to write  Only 0-255 are valid
} PORT_WRITE_STRUCT, *PPORT_WRITE_STRUCT;
