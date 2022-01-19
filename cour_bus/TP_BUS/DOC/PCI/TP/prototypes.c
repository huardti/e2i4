HANDLE  CloseTVicHW32(HANDLE HW32);

HANDLE  OpenTVicHW32(HANDLE HW32,
		     char * ServiceName,
		     char * EntryPoint);

BOOL    GetActiveHW(HANDLE HW32); 

UCHAR   GetPortByte(HANDLE HW32,ULONG PortAddr); 

USHORT GetLastPciBus(HANDLE HW32);
BOOL   GetPciDeviceInfo(HANDLE              HW32,
	    		USHORT              BusNum,
			USHORT              DeviceNum,
                        USHORT              FuncNum,
                        PPCI_COMMON_CONFIG  pPciCfg);