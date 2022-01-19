/****************************************************************************/
/*                                                                          */
/* Software Title: Example servo motor routine for AIM104-MOTION-1          */
/* Module Name:    DCSERVO3.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        DCSERVO3.C - AIM104-MOTION-1 example program for brushed */
/*                              dc servo motor in Proportional Velocity     */
/*										  mode.		 											 */
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
int axis_0;
unsigned long int programmed_vel,actual_vel;
const char filter_zero = 0;
const char filter_pole = 0;
const char filter_gain = 64;
unsigned char mode;

	clrscr();
	printf("SERVO3 PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	// Initialize motor type
	MOTION_set_board_config_reg(axis_0,0x00);		// DC servo motor
	printf(" Configuration register initialised... \n");

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
		printf(" Filter initialised... \n");

		// initialize timer
		MOTION_program_sample_timer(axis_0,127);  			// 1.024 ms
		printf(" Sample period set... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x01); //PWM Sign Reversal Inhibit
		printf(" Controller status initialised... \n");

		// initialise command position
		MOTION_set_command_position(axis_0,0);
		printf(" Command position initialised... \n");

	printf("Inititialisation complete. \n\n");

	MOTION_enter_position_mode(axis_0);
	printf("Motor in Position Control mode. \n");

	// set command velocity
	MOTION_set_proportional_velocity(axis_0,164);// 10.25(164/16) cts/sample time
																// => 300 rpm
	MOTION_execute_proportional_velocity(axis_0);
	printf("Proportional velocity program running, press any key to stop.\n\n");
	programmed_vel = 300;

		printf(" Programmed velocity =  \n");
		printf(" Actual velocity	  =  \n");

	if (kbhit())
		getch();

	while (!kbhit())
	{
		actual_vel = (MOTION_read_actual_velocity(axis_0))*29.297;
								// 29.297 converts quadcounts/sample time to rpm, if:
								// sample time = 1.024ms & quadcounts/rev. = 2000.

		gotoxy(23,12);		printf("%u rpm   ",programmed_vel);
		gotoxy(23,13);		printf("%u rpm   \n",actual_vel);

		if (kbhit()) break;
	}
   getch();

	printf("Program halted.");

	MOTION_end_proportional_velocity(axis_0);
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
