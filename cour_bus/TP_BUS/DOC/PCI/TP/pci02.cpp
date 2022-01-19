// Accès direct aux cartes PCI sous DOS et WIN95-98

#include <iostream.h>           
#include <stdio.h>             
#include <conio.h>
#include <dos.h>

#define 	TRUE	1
#define	FALSE	0

// recherche de la carte spécifiée par vendor_id et device_id  =======

unsigned int get_pci_io(unsigned vendor_id, unsigned device_id, char print_info)
{
	union 			REGS inregs, outregs;
	unsigned int 	bus;
	unsigned int 	slot_no;
	unsigned 		io_adr;
	io_adr 		= 0;

	for (bus = 0; (bus < 7 ) && (io_adr == 0); bus++)
	{
		for (slot_no=0;(slot_no<=0x00F8) && (io_adr==0);slot_no+=8)
		{
			inregs.x.bx = (bus << 8) | slot_no;
			inregs.x.di = 0x00; //  0x00 : read vendor ID
			inregs.x.ax = 0xb109;
			int86(0x1a, &inregs, &outregs);

			if (outregs.x.cx == vendor_id )
			{
				inregs.x.bx = (bus << 8) | slot_no;
				inregs.x.di = 0x02; //0x02 : read Device ID
				inregs.x.ax = 0xb109;
				int86(0x1a, &inregs, &outregs);

				if (outregs.x.cx == device_id)
				{
					inregs.x.ax = 0xb109;
					inregs.x.bx = (bus << 8) | slot_no;
					inregs.x.di = 0x10;	// Ox10 : read base address 0
					int86(0x1a, &inregs, &outregs);
					io_adr = outregs.x.cx;
					io_adr &= 0xfff0; 	/* masquage I/O-Adresse */

					if(print_info)
						printf("\nbus : 0x%x slot : 0x%x vendor_id : 0x%x device_id : 0x%x io_adr : 0x%x", bus, slot_no, vendor_id, device_id, io_adr);
				}
			}
		}
	}
	return (io_adr);
}



int A,B;
unsigned  adr;
unsigned device;
char	found;

//================================
main()
{
found = FALSE;
printf ("\nRecherche des cartes PCI VENDOR_ID = 0x1001 DEVICE_ID = 0x0000..0x017:\n");
for (device = 0; device <= 0x17; device++)
{
	adr=get_pci_io(0x1001,device,TRUE);
	if(adr !=0) found = TRUE;
}

if (! found) printf("\n Aucune carte avec ces identificateurs!");

{

printf ("\n\nappuyez sur une touche pour quitter");
do
{
} while (!kbhit()); // 	attente appui sur une touche du clavier

 return 0;
}}