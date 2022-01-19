/****************************************************************************/
/*                                                                          */
/* Software Title: Example stepper motor routine for AIM104-MOTION-1        */
/* Module Name:    STEPPER4.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        STEPPER4.C - AIM104-MOTION-1 example program for stepper */
/*                              motor in Integral Velocity mode, using half */
/*										  steps.    											 */
/*						 Note: 	200 steps/rev. stepper motor							 */
/*									500 encoder counts (2000 quad counts) per rev.   */
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
/* 21/07/98 1.1      Updated for v1I2 release                Richard Beaman */
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
int axis_0,i,speed;
const char filter_zero = 200;
const char filter_pole = 128;
const char filter_gain = 16;
unsigned char mode;

	clrscr();
	printf("STEPPER4 PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	MOTION_controller_reset(axis_0);
	// Must wait until controller has finished resetting
	mode = (MOTION_read_controller_status_reg(axis_0) & 0x20);
	while(mode!=0x20)
	{
		mode = (MOTION_read_controller_status_reg(axis_0) & 0x20);
	}

	MOTION_clear_interrupts(axis_0);

	// Initialize motor type
	MOTION_set_board_config_reg(axis_0,0x03);		// stepper motor, half-step
	printf(" Configuration register initialised... \n");


	// INITIALIZE CONTROLLER
		// initialize filter
		MOTION_set_filter_parameters(axis_0,filter_zero,filter_pole,filter_gain);
		printf(" Filter initialised... \n");

		// initialize timer
		MOTION_program_sample_timer(axis_0,127);   			// 1.024 ms
		printf(" Sample period set... \n");

		// initlialize commutator
		MOTION_program_commutator(axis_0,0,40,5,5,0,0);    // Ring=40
		printf(" Commutator initialised... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x02);		// 4 phase commutator
		printf(" Controller status initialised... \n");

		// initilaise command position
		MOTION_set_command_position(axis_0,0);
		MOTION_preset_actual_position(axis_0,0);
		printf(" Command position initialised... \n");

	printf("Inititialisation complete. \n");

	printf("Entering align mode...");
	MOTION_enter_align_mode(axis_0);
	delay(5000);				 // align mode takes 4096 sample periods to complete
	printf("   Completed.");

	// set acceleration
	MOTION_set_acceleration(axis_0,1);	   // 1 => 0.004 quadcts/(sample time*2)
														// which equates to 125 rpm/sec
	// set maximum velocity
	MOTION_set_integral_velocity(axis_0,0);

	// start motor running
	MOTION_execute_integral_velocity(axis_0);
	printf("\nIntegral velocity program running, press any key to stop.\n\n");
	speed=0;

	if (kbhit())
		getch();

	while (!kbhit())
	{
		if (speed==500)
		{
			speed=-500;
			MOTION_set_integral_velocity(axis_0,0xF5);// -10 quadcts/(sample time)
		}															// => -293rpm
		else
		{
			speed=500;
			MOTION_set_integral_velocity(axis_0,10);  // 10 quadcts/(sample time)
		}															// => 293rpm
		for(i=0;i<3000;i++)
		{
			delay(1);
			if (kbhit()) break;
		}
	}
	getch();

	printf("Program halted.");
	MOTION_set_integral_velocity(axis_0,0);
	delay(2000);
	MOTION_end_integral_velocity(axis_0);
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
