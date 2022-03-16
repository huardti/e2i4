// ------------------------------------------------------------------
// UNIT-PROGRAMM FÜR PCI-KARTEN  (c) 1999 by KOLTER ELECTRONIC
// ------------------------------------------------------------------
// include-file: pciio.h
// ------------------------------------------------------------------

#include <dos.h>
#define VENDOR_KOLTER  0x1001

// ==================================================================
unsigned int get_pci_io(unsigned PRODUKT_ID_KOLTER)
{
union 		REGS inregs, outregs;
unsigned int 	bus;
unsigned int 	slot_no;
unsigned 	io_adr;
io_adr 		= 0;

// alle Produkt-ID´s
for (PRODUKT_ID_KOLTER = 0; (PRODUKT_ID_KOLTER < 256 ); PRODUKT_ID_KOLTER++)
{
// such auf dem bus
  for (bus = 0; (bus < 7 ) && (io_adr == 0); bus++)
  {
  	for (slot_no=0;(slot_no<=0x00F8) && (io_adr==0);slot_no+=8)
  	{
	inregs.x.bx = (bus << 8) | slot_no;
  	inregs.x.di = 0x00; // 0 = read vendor
	inregs.x.ax = 0xb109;
	int86(0x1a, &inregs, &outregs);

// ist KOLTER Karte mit Vendor-ID 
	if (outregs.x.cx == VENDOR_KOLTER )
  	{                     
	inregs.x.bx = (bus << 8) | slot_no;
	inregs.x.di = 0x02; // 2 = read device id
	inregs.x.ax = 0xb109;
	int86(0x1a, &inregs, &outregs);

// ist KOLTER Produkt-ID
	if (outregs.x.cx == PRODUKT_ID_KOLTER)
	{
	inregs.x.ax = 0xb109;
	inregs.x.bx = (bus << 8) | slot_no;
	inregs.x.di = PRODUKT_ID_KOLTER;
	int86(0x1a, &inregs, &outregs);
	io_adr = outregs.x.cx;
	}
  	}
  	}
  }
}
io_adr &= 0xfff0; 	/* I/O-Adresse wird maskiert */
return (io_adr);
}

// ==================================================================
warte(unsigned long us)	 // mikrosekunden warten
{
unsigned long count;
count = 0;
for (count=0; count<us; count++)
 {
  inportb(0x0080);	// ca. 0,5 us dauer auf Pentium 200
  inportb(0x0080);      // ca. 0,5 us dauer auf Pentium 200
 }
return (0);
}

// ==================================================================