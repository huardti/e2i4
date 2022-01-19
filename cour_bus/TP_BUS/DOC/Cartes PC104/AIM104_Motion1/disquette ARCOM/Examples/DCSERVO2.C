/****************************************************************************/
/*                                                                          */
/* Software Title: Example servo motor routine for AIM104-MOTION-1          */
/* Module Name:    DCSERVO2.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        DCSERVO2.C - AIM104-MOTION-1 example program for brushed */
/*                              DC servo motor in Trapezoidal Profile mode. */
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
/* 13/07/98 1.1      Updated for V1I2 release                Richard Beaman */
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
long int i;
unsigned char in_profile;
const char filter_zero = 229;
const char filter_pole = 128;
const char filter_gain = 8;
const long int start_position = 0;
long int actual_position;
unsigned char mode;

	clrscr();
	printf("SERVO2 PROGRAM RUNNING... \n");

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
		MOTION_program_sample_timer(axis_0,63);  	  // 0.512 ms
		printf("Sample period set... \n");

		// set controller status register
		MOTION_set_controller_status_reg(axis_0,0x01); //PWM Sign Reversal Inhibit
		printf("Controller status initialised... \n");

		// set command position (and actual position)
		MOTION_preset_actual_position(axis_0,start_position);
		MOTION_set_command_position(axis_0,start_position);
		printf("Position initialised... \n");

		// Set to position mode
		MOTION_enter_position_mode(axis_0);
		printf("Motor in Position Control mode. \n");

		// set acceleration
		MOTION_set_acceleration(axis_0,1);      		// 0.004 cts/(sample time*2)
		printf("Acceleration initialised... \n");    // 447 rpm/sec

		// set maximum velocity
		MOTION_set_max_velocity(axis_0,10);          // 10 cts/sample time
		printf("Maximum velocity set... \n");        // 586 rpm

	printf("Inititialisation complete. \n\n");


	printf("Trapezoidal profile program running, press any key to stop.\n");
	if (kbhit())
		getch();


	while (!kbhit())
	{

		for(i=1;i<11;i++)
		{
			MOTION_set_final_position(axis_0,10000*i);		// set profile move
			MOTION_execute_trapezoidal_move(axis_0);			// start move

			in_profile = (MOTION_read_board_status_reg(axis_0) & 0x20);
			// wait until profile complete
			while(in_profile)
			{
				in_profile = (MOTION_read_board_status_reg(axis_0) & 0x20);
			}

			delay(200);
			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,11);		printf("%li quad counts           ",actual_position);

			delay(300);
			if (kbhit()) break;
		}
		if (kbhit()) break;
		for(i=9;i>=0;i--)
		{
			MOTION_set_final_position(axis_0,10000*i);
			MOTION_execute_trapezoidal_move(axis_0);

			in_profile = (MOTION_read_board_status_reg(axis_0) & 0x20);
			// wait until profile complete
			while(in_profile)
			{
				in_profile = (MOTION_read_board_status_reg(axis_0) & 0x20);
			}

			delay(200);
			actual_position = MOTION_read_actual_position(axis_0);
			gotoxy(1,11);		printf("%li quad counts           ",actual_position);

			delay(300);
			if (kbhit()) break;
		}
	}
	getch();

	printf("\n\nProgram halted.");
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
