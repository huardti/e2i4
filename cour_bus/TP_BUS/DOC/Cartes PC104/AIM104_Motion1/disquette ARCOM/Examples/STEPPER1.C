/****************************************************************************/
/*                                                                          */
/* Software Title: Example stepper motor routine for AIM104-MOTION-1        */
/* Module Name:    STEPPER1.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        STEPPER1.C - AIM104-MOTION-1 example program for stepper */
/*                              motor in Position Control mode, using half  */
/*										  steps.  Program assumes stepper has 200     */
/*										  per rev and encoder 2000 quad cts per rev.  */
/*                                                                          */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.5 / V3.1                            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 20/04/98 1.0      First release                           Richard Beaman */
/* 21/07/98 1.1      Updated for V1I2 release                Richard Beaman */
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
const char filter_zero = 128;
const char filter_pole = 128;
const char filter_gain = 16;
const long int start_position = 0;
unsigned long int actual_position;
unsigned char mode;

	clrscr();
	printf("STEPPER1 PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	// Initialize motor type
	MOTION_set_board_config_reg(axis_0,0x03);		// stepper motor, half-step
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
		MOTION_program_sample_timer(axis_0,127);				 // 1.024 ms
		printf("Sample period set... \n");

		// initlialize commutator
		MOTION_program_commutator(axis_0,0,40,5,5,0,0);		 // Ring = 40
		printf("Commutator initialised... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x02);		 // 4 phase commutator
		printf("Controller status initialised... \n");

		// set command position (and actual position)
		MOTION_preset_actual_position(axis_0,start_position);
		MOTION_set_command_position(axis_0,start_position);
		printf("Position initialised... \n");
	printf("Inititialisation complete. \n");

	printf("Entering align mode...");
	MOTION_enter_align_mode(axis_0);
	delay(5000);				 // align mode takes 4096 sample periods to complete
	printf("   Completed.");

	printf("\n\nMotor control program running, press any key to stop.\n");
	if (kbhit())
		getch();

	while (!kbhit())
	{
		for(i=1;i<11;i++)
		{
			MOTION_set_command_position(axis_0,2000*i);
			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,12);		printf("%lu quad counts           ",actual_position);
			delay(1000);
			if (kbhit()) break;
		}
		for(i=9;i>=0;i--)
		{
			MOTION_set_command_position(axis_0,2000*i);
			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,12);		printf("%lu quad counts           ",actual_position);
			delay(1000);
			if (kbhit()) break;
		}
	}
	getch();

	printf("\nProgram halted.");
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
