/****************************************************************************/
/*                                                                          */
/* Software Title: Example DC servo motor routine for AIM104-MOTION-1       */
/* Module Name:    DCSERVO1.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        DCSERVO1.C - AIM104-MOTION-1 example program for brushed */
/*                              DCservo motor in Position Control mode.		 */
/*                                                                          */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.52 / V3.1                           */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 15/05/98 1.0      First release                           Richard Beaman */
/* 13/07/98 1.1      V1I2 release                            Richard Beaman */
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
const char filter_zero = 229;
const char filter_pole = 128;
const char filter_gain = 12;
long int start_position=0;
long int actual_position;
unsigned char mode;

	clrscr();
	printf("DCSERVO1 PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	// Initialize motor type
	MOTION_set_board_config_reg(axis_0,0x00);		// DC servo motor
	printf("Configuration register initialised... \n");

	MOTION_controller_reset(axis_0);
	// Must wait until controller has finished resetting
	mode = (MOTION_read_controller_status_reg(axis_0) & 0x20);
	while(mode!=0x20)
	{
		mode = (MOTION_read_controller_status_reg(axis_0) & 0x20);
	}

	MOTION_clear_interrupts(axis_0);


	// INITIALIZE CONTROLLER
		// initialize filter
		MOTION_set_filter_parameters(axis_0,filter_zero,filter_pole,filter_gain);
		printf("Filter initialised... \n");

		// initialize timer
		MOTION_program_sample_timer(axis_0,7);		  // 64 us
		printf("Sample period set... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x03); //PWM Sign Reversal Inhibit
		printf("Controller status initialised... \n");

		// set command position (and actual position)
		MOTION_preset_actual_position(axis_0,start_position);
		MOTION_set_command_position(axis_0,start_position);
		printf("Position initialised... \n");
	printf("Inititialisation complete. \n\n");

		actual_position = MOTION_read_actual_position(axis_0);
		gotoxy(1,11);		printf("%li quad counts              ",actual_position);

	// Set to position mode
	MOTION_enter_position_mode(axis_0);
	gotoxy(1,10);	printf("Motor in Position Control mode. \n");

	printf("\n\nMotor control program running, press any key to stop.\n");
	if (kbhit())
		getch();

	while (!kbhit())
	{

			for(i=1;i<11;i++)
		{
			MOTION_set_command_position(axis_0,2000*i);

			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,11);		printf("%li quad counts           ",actual_position);

			delay(1000);
			if (kbhit()) break;
		}
		for(i=9;i>=0;i--)
		{
			MOTION_set_command_position(axis_0,2000*i);

			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,11);		printf("%li quad counts              ",actual_position);

			delay(1000);
			if (kbhit()) break;
		}
	}
	getch();

	printf("\n\n\n\n\nProgram halted.");
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
