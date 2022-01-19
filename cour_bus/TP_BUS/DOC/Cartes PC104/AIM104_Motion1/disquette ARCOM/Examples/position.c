/****************************************************************************/
/*                                                                          */
/* Software Title: Test routine for AIM104-MOTION-1    						    */
/* Module Name:    POSITION.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        POSITION.C - AIM104-MOTION-1 test program to display the */
/*                              real-time encoder count value.        		 */
/*                                                                          */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.52 / V3.1                            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 30/04/98 1.0      First release                           Richard Beaman */
/****************************************************************************/

/* - Include Files -------------------------------------------------------- */
#include <stdio.h>;
#include <conio.h>;
#include <stdlib.h>;
#include <dos.h>;
#define MAIN
#include "motion.h";

/* - Global Variables ----------------------------------------------------- */


/* - Main Program --------------------------------------------------------- */
void main()
{
unsigned short BoardAddr;
int axis_0,i;
long int actual_position;

	clrscr();
	printf("\n\nPOSITION PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	// reset encoder count to zero
	MOTION_preset_actual_position(axis_0,0);
	printf("preset position complete!\n");

	MOTION_set_board_config_reg(axis_0,0x00);
	i = MOTION_read_board_config_reg(axis_0);
	printf("\n select(1:0) = %i",i);
	i = MOTION_read_board_status_reg(axis_0);
	printf("\n status reg  = %i",i);


	actual_position = MOTION_read_actual_position(axis_0);

	printf("\n\n Encoder count =  \n");
	printf("\n\nPress any key to stop.\n");
	if (kbhit())
		getch();

	while (!kbhit())
	{
		actual_position = MOTION_read_actual_position(axis_0);
		gotoxy(18,9);		printf("%lu quad counts              ",actual_position);
		if (kbhit()) break;
	}
	getch();

	gotoxy(1,13);	printf("Program halted.");
	MOTION_boardclose(axis_0);

	exit(1);
}
