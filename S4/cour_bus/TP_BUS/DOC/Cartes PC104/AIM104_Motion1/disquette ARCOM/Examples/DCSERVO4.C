/****************************************************************************/
/*                                                                          */
/* Software Title: Example DC servo motor routine for AIM104-MOTION-1       */
/* Module Name:    DCSERVO4.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        DCSERVO4.C - AIM104-MOTION-1 example program for brushed */
/*                              DC servo motor in Integral Velocity mode.   */
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
/* 15/05/98 1.0      First release                           Richard Beaman */
/* 15/05/98 1.1      Updated for V1I2 release                Richard Beaman */
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
int axis_0,speed;
const char filter_zero = 229;
const char filter_pole = 128;
const char filter_gain = 4;
unsigned char mode;

	clrscr();
	printf("DCSERVO4 PROGRAM RUNNING... \n");

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
	MOTION_set_board_config_reg(axis_0,0x00);		// DC servo motor
	printf(" Configuration register initialised... \n");


	// INITIALIZE CONTROLLER
		// initialize filter
		MOTION_set_filter_parameters(axis_0,filter_zero,filter_pole,filter_gain);
		printf(" Filter initialised... \n");

		// initialize timer
		MOTION_program_sample_timer(axis_0,63);   	  // 0.512 ms
		printf(" Sample period set... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x01); //PWM SIgn Reversal Inhibit
		printf(" Controller status initialised... \n");

		// initilaise command position
		MOTION_set_command_position(axis_0,0);
		MOTION_preset_actual_position(axis_0,0);
		printf(" Command position initialised... \n");

	printf("Inititialisation complete. \n\n");

	// set to position mode
	MOTION_enter_position_mode(axis_0);

	// set acceleration
	MOTION_set_acceleration(axis_0,2);	   // 2 => 0.008 quadcts/(sample time*2)
														// which equates to 250 rpm/sec
	// set maximum velocity
	MOTION_set_integral_velocity(axis_0,0x08); //8 quadcts/sample time = 468 rpm

	// start motor running
	MOTION_execute_integral_velocity(axis_0);
	printf("Integral velocity program running, press any key to stop.\n\n");
	speed=-500;

	if (kbhit())
		getch();
	while (!kbhit())
	{
		if (speed==500)
		{
			speed=-500;
			MOTION_set_integral_velocity(axis_0,0xF0);// -15 quadcts/(sample time)
			delay(5000);
		}															// => -439rpm
		else
		{
			speed=500;
			MOTION_set_integral_velocity(axis_0,15);  // 15 quadcts/(sample time)
			delay(5000);
		}															// => 439rpm

	}

	printf("Program halted.");
	delay(3000);
	MOTION_end_integral_velocity(axis_0);
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
