#include <windows.h>
#include <iostream.h>

#include <iostream.h>
#include <stdio.h>             
#include <conio.h>

#include "Klibdrv8.h"
#include <dos.h>

#define 	TRUE	1
#define	FALSE	0
#define MAXMODULE 50

//============= Interface avec la DLL : KLIBDRV.DLL =========================

//HANDLE  VICFN OpenTVicHW32(HANDLE HW32, char * ServiceName, char * EntryPoint);
typedef  HANDLE (WINAPI*cfunc)(HANDLE, char *, char *);
cfunc OpenTVicHW32;
//BOOL    VICFN GetActiveHW(HANDLE HW32);
typedef BOOL   (WINAPI*cfunc1)(HANDLE);
cfunc1	GetActiveHW;
//USHORT VICFN GetLastPciBus(HANDLE HW32);
typedef	USHORT  (WINAPI*cfunc2)(HANDLE);
cfunc2  GetLastPciBus;
/*
BOOL   VICFN GetPciDeviceInfo( HANDLE              HW32,
	    					   USHORT              BusNum,
							   USHORT              DeviceNum,
                               USHORT              FuncNum,
										 PPCI_COMMON_CONFIG  pPciCfg);
*/
typedef BOOL (WINAPI*cfunc3)( HANDLE, USHORT, USHORT, USHORT, PPCI_COMMON_CONFIG);
cfunc3		GetPciDeviceInfo;

//UCHAR   VICFN GetPortByte(HANDLE HW32,ULONG PortAddr);

//void    VICFN SetPortByte(HANDLE HW32,ULONG PortAddr, UCHAR nNewValue);
typedef void  (WINAPI* cfunc4)(HANDLE,ULONG, UCHAR);
cfunc4 SetPortByte;

//HANDLE  VICFN CloseTVicHW32(HANDLE HW32);
typedef HANDLE	(WINAPI*cfunc6)(HANDLE);
cfunc6 CloseTVicHW32;

//=================Variables ===================================

HANDLE	HwCtrl=NULL;
BOOL		ActiveHW    = FALSE;
PCI_COMMON_CONFIG Info;
WORD  	buses,n,bus,dev,func;
ULONG		base[10] ;
int i;
char	theChar;

//================================
main()
{

	for(i=0;i<=9;i++) base[i] = 0;

	printf ("\nProgramme de test des cartes KOLTER sous WIN NT avec la DLL KLIBDRV.DLL\n\n");

	// Chargement de la DLL

	HINSTANCE hLib=LoadLibrary("KLIBDRV.DLL");
	if(hLib==NULL)
	{
			  printf("\nChargement DLL impossible!");
			  getch();
			  return (-1);
	}

	char mod[MAXMODULE];

	GetModuleFileName((HMODULE)hLib, (LPTSTR)mod, MAXMODULE);
	printf("DLL chargee : %s\n", mod);
	printf("Fonctions :\n", mod);
	OpenTVicHW32 		= (cfunc)GetProcAddress((HMODULE)hLib, "OpenTVicHW32");
	GetActiveHW			=(cfunc1)GetProcAddress((HMODULE)hLib, "GetActiveHW");
	GetLastPciBus		=(cfunc2)GetProcAddress((HMODULE)hLib, "GetLastPciBus");
	GetPciDeviceInfo 	= (cfunc3)GetProcAddress((HMODULE)hLib, "GetPciDeviceInfo");
	SetPortByte			= (cfunc4)GetProcAddress((HMODULE)hLib, "SetPortByte");
	CloseTVicHW32		= (cfunc6)GetProcAddress((HMODULE)hLib, "CloseTVicHW32");
	printf("OpenTVicHW32@=\t\t0x%lx\n",OpenTVicHW32);
	printf("CloseTVicHW32@=\t\t0x%lx\n",CloseTVicHW32);
	printf("GetActiveHW@=\t\t0x%lx\n",GetActiveHW);
	printf("GetLastPciBus@=\t\t0x%lx\n",GetLastPciBus);
	printf("GetPciDeviceInfo@=\t0x%lx\n",GetPciDeviceInfo);
	printf("SetPortByte@=\t\t0x%lx\n",SetPortByte);

	if((OpenTVicHW32==NULL)|| GetActiveHW == NULL )
	{
		printf("Unable to load function(s).");
		FreeLibrary((HMODULE)hLib);
		return (-1);
	}

	// Chargement du Driver KLIBDRV.SYS

	printf("\nChargement du Driver : ");
	HwCtrl = OpenTVicHW32(HwCtrl,"KLIBDRV","KLIBDevice0");
	if (!HwCtrl || !GetActiveHW(HwCtrl))
	{
		printf("\impossible!");
	}
	else
	{
		printf("\OK!");
		ActiveHW = TRUE;
		printf("\tHwCtrl = 0x%lx\n",HwCtrl);

		printf("\nScrutation du bus PCI pout trouver les cartes KOLTER : \n");

		buses = GetLastPciBus(HwCtrl);
		printf("\nbuses=%lx\n",buses);
		n = 0;
    
		if (buses != 0xFFFF)	//buses>=0
		{
			for (bus = 0; bus<=buses; bus++)
			{
				for (dev = 0; dev<32; dev++)
				{
					for (func= 0; func<8; func++)
					{
						if ( GetPciDeviceInfo(HwCtrl,bus,dev,func,&Info) &&
								 (Info.VendorID == 0x1001) &&
								 (Info.DeviceID != 0xFFFF) )
						{
							n++;
							base[n] = (Info.u.type0.BaseAddresses[0]-1);
							printf("carte %d Bus = 0x%x DeviceID = 0x%x  BaseAddr = 0x%x\n",n, bus, Info.DeviceID, base[n]);
						}
					}

				}
			}
		}

		if(n >=1)
		{
			printf ("\n\nclignotement des LEDS-appuyer sur une touche pour arreter");
			while ( !kbhit() )
			{
				SetPortByte(HwCtrl,base[1]+8, 0x00);
				if(n>1) SetPortByte(HwCtrl,base[2]+8, 0x00);
				for(i=0;i<=10000000;i++);
				SetPortByte(HwCtrl,base[1]+8, 0x01);
				if(n>1) SetPortByte(HwCtrl,base[2]+8, 0x01);
				for(i=0;i<=1000000;i++);
			}
		}
		getch();

	}//else driver != OK

	printf ("\n\nappuyez sur une touche pour quitter");
	do
	{
	} while (!kbhit()); // 	attente appui sur une touche du clavier

	// Fermeture ....

	ActiveHW=FALSE;
	HwCtrl=CloseTVicHW32(HwCtrl);
	FreeLibrary((HMODULE)hLib);
	return 0;
}