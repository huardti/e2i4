/****************************************************************************/
/*                                                                          */
/* Software Title: Example servo motor routine for AIM104-MOTION-1          */
/* Module Name:    DCSERVO5.C                                               */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        DCSERVO5.C - AIM104-MOTION-1 example program for brushed */
/*                              dc servo motor in Proportional Velocity     */
/*										  mode using interrupts to reverse motor upon */
/*                              encountering a limit                        */
/* Language:       ANSI C                                                   */
/* Compiler:       Borland C Version V4.52 / V3.1                           */
/*                 Based on 'dcservo3' by Richard Beaman                                                         */
/* ======================================================================== */
/*                                                                          */
/* Modification history                                                     */
/*                                                                          */
/* Date     Version  Description                              Author        */
/* ------------------------------------------------------------------------ */
/* 16/07/98 1.1      First release                           Abe Sharp      */
/****************************************************************************/

/* - Include Files -------------------------------------------------------- */
#include <stdio.h>;
#include <conio.h>;
#include <stdlib.h>;
#include <dos.h>;
#define MAIN
#include "motion.h";
#include "pcints.h";

#define FALSE 0
#define TRUE !FALSE

/* - Declaration of interrupt functions------------------------------------ */
void Reverse_Motor (void);
void interrupt ISR_LIMIT (void);
void interrupt (*oldISR) (void);

/* - Global Variables ----------------------------------------------------- */
unsigned short BoardAddr;
int target_vel;
int axis_0;
int LIMIT_FLAG = FALSE ; // Global flag set by limit ISR and reset upon service completion
/* - Main Program --------------------------------------------------------- */

void main()
{
int actual_vel;
const char filter_zero = 0;
const char filter_pole = 0;
const char filter_gain = 150;
unsigned char mode;

	clrscr();
	printf("SERVO5 PROGRAM RUNNING... \n");

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

	printf(" Enter target velocity in RPM : ");
	scanf("%i", &target_vel);


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

	//Start motor off
	MOTION_set_proportional_velocity(axis_0,target_vel*0.546);// derived from formula
	MOTION_enter_position_mode(axis_0);																				  // on manual page 20
	MOTION_execute_proportional_velocity(axis_0);

	printf("Trigger limit switch to reverse motor.\n\n");
	oldISR = Install(ISR_LIMIT, IRQ5); // set IRQ5's interrupt vector

		printf(" Target velocity =  \n");
		printf(" Actual velocity =  \n");

	if (kbhit())
		getch();

	while (!kbhit())
	{
		actual_vel = (MOTION_read_actual_velocity(axis_0))*29.297;
								// 29.297 converts quadcounts/sample time to rpm, if:
								// sample time = 1.024ms & quadcounts/rev. = 2000.

		gotoxy(20,12);		printf("%i rpm   ",target_vel);
		gotoxy(20,13);		printf("%i rpm   \n",actual_vel);

		if (LIMIT_FLAG) Reverse_Motor() ;

		if (kbhit()) break;
	}

	DeInstall (oldISR, IRQ5);	// Restore interrupt vector

	getch();

	printf("\nProgram halted.");

	MOTION_end_proportional_velocity(axis_0);
	MOTION_enter_idle_mode(axis_0);
	MOTION_boardclose(axis_0);
	exit(1);
}

void Reverse_Motor (void)
{

		// set command velocity
		target_vel *=-1;			// invert programmed velocity

		if (target_vel >= 0) {
			gotoxy(0,15);  printf("Motor going forwards ");
			}

		else {
			gotoxy(0,15);  printf("Motor going backwards");
			}

		MOTION_set_proportional_velocity(axis_0,target_vel*0.546);// derived from formula
																					  // on manual page 20
		MOTION_enter_position_mode(axis_0);

		MOTION_execute_proportional_velocity(axis_0);

		LIMIT_FLAG = FALSE ;
}



void interrupt ISR_LIMIT (void)      //this ISR is executed when an interrupt is received on IRQ5
{
	disable ();

	if (!(MOTION_read_board_status_reg(axis_0) & 0x02)) {

		outportb(BoardAddr, 0x09);						// PWM command port
		delay(10);

		if (target_vel >= 0) outportb(BoardAddr+1, -10);      // Back off the limit switch
		else outportb(BoardAddr+1, 10);

		while (!(MOTION_read_board_status_reg(axis_0) & 0x02))
			MOTION_clear_interrupts(axis_0);		//wait until limit switch 1 is no longer active

		outportb(BoardAddr+1, 0);                 //stop backing off now limit switch is cleared

		LIMIT_FLAG = TRUE ;

	}

	outportb(OCW4, EOI);
	outportb(OCW2, EOI);
	oldISR();
	enable();
}


void far *Install(void interrupt (*faddr)(), int vect_num)
{
	int mask_bit;
	void interrupt (*oldVect)(void);

	disable();
	oldVect = getvect(vect_num);                /* Save old interrupt vectors */
	setvect(vect_num, faddr);
	if (vect_num <= 15)                               /* Enable interrupt... */
	{
		 mask_bit = vect_num - 8;
		 outportb(OCW1, inportb(OCW1) & ~(1 << mask_bit));
	}
	else
	{
		 mask_bit = vect_num - 112;
		 outportb(OCW3, inportb(OCW3) & ~(1 << mask_bit));
		 outportb(OCW1, inportb(OCW1) & 0xfb);        /* Enable IRQ2 on PIC 1 */
	}
	enable();
	return *oldVect;
}


void DeInstall(void interrupt (*oldISR)(), int vect_num)
{
	 int mask_bit;

	 disable();
	 if (vect_num <= 15)
	 {
		  mask_bit = vect_num - 8;
		  outportb(OCW1, inportb(OCW1) | (1 << mask_bit));
	 }
	 else
	 {
		  mask_bit = vect_num - 112;
		  outportb(OCW3, inportb(OCW3) | (1 << mask_bit));
	 }
	 setvect(vect_num, oldISR);                        /* Re-install old ISR */
	 enable();
}
/* EOF */

