/****************************************************************************/
/*                                                                          */
/* Software Title: PCIB Library for Arcom PCIB40 and related products       */
/* Module Name:    arcom.h                                                  */
/*                 Copyright (c) 1995 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        globals.h - Constants and directives common to all       */
/*                             modules.                                     */
/*                                                                          */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.5 / Microsoft VC++ 1.5              */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 01/11/95 1.0      First release                            Craig N Grove */
/*                                                                          */
/****************************************************************************/


// Make sure 'globals.h' is included once only
#ifndef GLOBS

#include "globals.h"
#define GLOBS

#endif


// Allow for Borland or Microsoft C
#if defined __BORLANDC__

	#include <dos.h>

#else

	#include <conio.h>

#endif


/* - Function Prototypes -------------------------------------------------- */

int ANALOG_write(int handle, unsigned short port, unsigned short val);
int ANALOG_read_channel(int handle, unsigned short chan);
int ANALOG_read(int handle);
int ANALOG_select_channel(int handle, unsigned short channel);
int ANALOG_select_gain(int handle, unsigned short gain);
int ANALOG_read_status(int handle);
int ANALOG_start_conversion(int handle);
int ANALOG_set_expansionaddr(int handle, int addr);

int DIGITAL_write(int handle, unsigned short port, unsigned char val);
int DIGITAL_read(int handle, unsigned short port);
int DIGITAL_write_bit(int handle, unsigned char bit, unsigned char val);
int DIGITAL_read_bit(int handle, unsigned char bit);
int DIGITAL_group_enable(int handle);
int DIGITAL_group_disable(int handle);
int DIGITAL_io_config(int handle, unsigned char config);

int COMMON_boardinit(unsigned short baseaddr, unsigned char id);
int COMMON_boardclose(int handle);
int COMMON_ledon(int handle);
int COMMON_ledoff(int handle);

void LOWLEV_boardwrite(unsigned short baseaddr, unsigned short offset, unsigned char value);
unsigned char LOWLEV_boardread(unsigned short baseaddr, unsigned short offset);
int LOWLEV_bitnum(unsigned char bitaddr);
void interrupt far LOWLEV_isr(void);
int LOWLEV_digital_read_debounce(int handle, unsigned short port);
int LOWLEV_digital_read_bit_debounce(int handle, unsigned short bit);
int LOWLEV_timer_init(int handle, unsigned short newcnt, int port, int mode);
int LOWLEV_timer_reset(int handle, unsigned short newcnt, int port, int mode);
int LOWLEV_timer_disable(int handle, int port);
int LOWLEV_timer_read(int handle, int port, unsigned *count);
int LOWLEV_timer_read_ctc_int(int handle);
int LOWLEV_timer_clr_ctc_int(int handle);
int LOWLEV_timer_trig_timebase(int handle);
int LOWLEV_timer_board_setup(int handle, int group_1to4, int group_5to8, int int_mode);

unsigned int LOWLEV_wordread(unsigned short baseaddr, unsigned short offset);
void LOWLEV_wordwrite(unsigned short baseaddr, unsigned short offset, unsigned int value);
int LOWLEV_analog_read_filtered(int handle, unsigned short port);
int LOWLEV_analog_read_latest(int handle, unsigned short port);

int SERIAL_setup_port(int, int, unsigned short, char parity, unsigned char, unsigned char);
int SERIAL_reset(int, int);
int SERIAL_flush(int, int);
int SERIAL_write(int, int, unsigned char*);
int SERIAL_read(int, int, unsigned char*, int);
int SERIAL_peekbyte(int, int);
int SERIAL_peekbyte_timed(int, int, double);
void interrupt far SERIAL_isr(void);

int SYSCON_read_status (int handle);
int SYSCON_read_mask (int handle, unsigned char port);
int SYSCON_set_mask (int handle, unsigned char enableMask, unsigned char port);
int SYSCON_reset_watchdog (int handle);
int SYSCON_set_watchdog (int handle, unsigned char timeOut);
int SYSCON_set_comparator (int handle, unsigned short port, unsigned short val);

#ifdef __BORLANDC__

int LOWLEV_interruptinit(int handle, unsigned char irq, void interrupt far (*Vector)());

#else

int LOWLEV_interruptinit(int handle, unsigned char irq, void(__interrupt __far *Vector)());

#endif

#ifndef __BORLANDC__

void clrscr(void);			// 25 printf's to clear the screen
void delay(unsigned short);             // Delay 'n' milliseconds

#define outportb outp
#define outport outpw

#define inportb inp
#define inport inpw

#define getvect _dos_getvect
#define setvect _dos_setvect
#define enable _enable
#define disable _disable

#define gotoxy(x, y) (void)_settextposition(y, x)
#endif


