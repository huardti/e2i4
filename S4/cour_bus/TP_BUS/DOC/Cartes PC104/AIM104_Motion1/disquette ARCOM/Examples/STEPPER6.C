/****************************************************************************/
/*                                                                          */
/* Software Title: Example stepper motor routine for AIM104-MOTION-1        */
/* Module Name:    STEPPER6.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        STEPPER6.C - AIM104-MOTION-1 example program for stepper */
/*                              motor in open-loop control using half-steps.*/
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
int axis_0;
const long int start_position = 0;

	clrscr();
	printf("STEPPER6 PROGRAM RUNNING... \n");

	BoardAddr = 0x180;
	axis_0 = MOTION_boardinit(BoardAddr);

	// Initialize motor type
	MOTION_set_board_config_reg(axis_0,0x03);		// stepper motor, half-step
	printf("Configuration register initialised... \n");

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

	printf("Inititialisation complete. \n\n");

	// Set the 'Hold commutator flag'
	MOTION_wait_until_ready(axis_0);
	outportb(BoardAddr, 0x00);						// Flag register
	MOTION_wait_until_ready(axis_0);
	outportb(BoardAddr+1, 0x0C);

	printf("Motor control program running, press any key to stop.\n");
	if (kbhit())
		getch();

	MOTION_wait_until_ready(axis_0);
	outportb(BoardAddr, 0x09);						// PWM command port
	MOTION_wait_until_ready(axis_0);
	outportb(BoardAddr+1,100);

	while (!kbhit())
	{
		// Steady speed
		MOTION_open_loop_half_stepper(axis_0,4000,70);
	}
	getch();

	printf("\nProgram halted.");
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}
