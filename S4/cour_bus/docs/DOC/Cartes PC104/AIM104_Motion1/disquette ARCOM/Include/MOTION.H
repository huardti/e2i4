/****************************************************************************/
/*                                                                          */
/* Software Title: PC104 Library for Arcom AIM104-MOTION-1 board  	       */
/* Module Name:    motion.h                                                 */
/*                 Copyright (c) 1998 Arcom Control Systems Ltd.            */
/*                                                                          */
/* ======================================================================== */
/*                                                                          */
/* Summary:        motion.h - AIM104-MOTION-1 Function prototypes           */
/*                                                                          */
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
/* 15/04/98 1.0      First release                           Richard Beaman */
/* 30/06/98 1.1      MOTION_read_sample_timer() added								 */
/*							Open loop stepper functions use sample						 */
/*							timer for delay, not delay() function						 */
/* 09/07/98 1.1 		All integers used in position functions 					 */
/*							defined as long   							 Richard Beaman */
/****************************************************************************/

//----------------------------------------------------------------------------
//
// FUNCTIONS DEFINED IN HEADER
//
// int MOTION_boardinit(unsigned short baseaddr);
// int MOTION_boardclose(int handle);
// void MOTION_wait_until_ready(int handle)
// void MOTION_set_board_config_reg(int handle, char config_value)
// char MOTION_read_board_config_reg(int handle)
// void MOTION_pulse_SYNC(int handle)
// void MOTION_set_controller_status_reg(int handle, char status)
// void MOTION_program_sample_timer(int handle, char period)
// void MOTION_set_filter_parameters(int handle,char filter_zero,
//											char filter_pole,char filter_gain)
// void MOTION_controller_reset(int handle)
// void MOTION_clear_interrupts(int handle)
// void MOTION_enter_idle_mode(int handle)
// void MOTION_enter_align_mode(int handle)
// void MOTION_enter_position_mode(int handle)
// void MOTION_execute_trapezoidal_move(int handle)
// void MOTION_execute_proportional_velocity(int handle)
// void MOTION_end_proportional_velocity(int handle)
// void MOTION_execute_integral_velocity(int handle)
// void MOTION_end_integral_velocity(int handle)
// char MOTION_wait_until_move_complete(int handle)
// void MOTION_set_command_position(int handle,unsigned long command_position)
// void MOTION_set_final_position(int handle,unsigned long final_position)
// void MOTION_preset_actual_position(int handle,unsigned long actual_position)
// void MOTION_set_acceleration(int handle, int accel)
// void MOTION_set_max_velocity(int handle, char velocity)
// void MOTION_set_proportional_velocity(int handle, int velocity)
// void MOTION_set_integral_velocity(int handle, char velocity)
// void MOTION_program_commutator(int handle,char velocity_timer,char ring,
//									char X,char Y,char offset,char max_phase_advance)
// char MOTION_read_board_status_reg(int handle)
// char MOTION_read_controller_status_reg(int handle)
// char MOTION_read_flag_register(int handle)
// unsigned long int MOTION_read_command_position(int handle)
// unsigned long int MOTION_read_final_position(int handle)
// unsigned long int MOTION_read_actual_position(int handle)
// int MOTION_read_actual_velocity(int handle)
// unsigned char MOTION_read_sample_timer(int handle)
// char MOTION_open_loop_half_stepper(int handle,int total_steps,
//																					int step_period)
//
//------------------------------------------------------------------------------

#include <dos.h>
#include "arcom.h"

#ifndef GLOBS
#include "globals.h"
#define GLOBS
#endif

#if !defined(__MOTION_H)
#define __MOTION_H

int MOTION_boardinit(unsigned short baseaddr)
{
	if (boardindex >= MAX_BOARDS)
		return(ER_TOO_MANY);

	boardstore[boardindex].base_addr = baseaddr;

	return(boardindex++);
}


int MOTION_boardclose(int handle)
{
	if ((handle < 0) || (handle >= boardindex))
		return(ER_BOARD_NOT_OPEN);

	//This board is no longer in use
	boardstore[handle].base_addr = 0;
   return(0);
}


void MOTION_wait_until_ready(int handle)
{
char status;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	status = inportb(baseaddr+2);
	while ((status&0x01)==0x01)
	{
		// controller busy
		status = inportb(baseaddr+2);				// AIM104-MOTION-1 status reg
	}
}



void MOTION_set_board_config_reg(int handle, char config_value)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;
	outportb(baseaddr+3,config_value);
}



char MOTION_read_board_config_reg(int handle)
{
char config_value;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	config_value = inportb(baseaddr+3);
	return(config_value);
}



void MOTION_pulse_SYNC(int handle)
{
char config_value;

	config_value = MOTION_read_board_config_reg(handle);
	config_value = config_value + 4;
	MOTION_set_board_config_reg(handle,config_value);
}



char MOTION_read_controller_status_reg(int handle)
{
char status;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x07);						// Status register
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	status = inportb(baseaddr+1);
	return(status);
}



void MOTION_set_controller_status_reg(int handle, char status)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x07);						// Status register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,status);					// Write status value - also
															// clears limit and stop flags
}



void MOTION_program_sample_timer(int handle, char period)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x0F);							// Sample timer register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,period);						// Write timer value
}



void MOTION_set_filter_parameters(int handle,char filter_zero,
														char filter_pole,char filter_gain)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x20);							// Filter zero register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,filter_zero);	  			// Write zero value
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x21);							// Filter pole register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,filter_pole);	  			// Write pole value
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x22);							// Filter gain register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,filter_gain);	  			// Write gain value

}



void MOTION_controller_reset(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x05);                    	// Program counter register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x00);                  	// software reset
}



void MOTION_clear_interrupts(int handle)
{
char status_value;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	// clear board level interrupt flags
	outportb(baseaddr+2,0);

	// clear HCTL-1100 controller status flags
	status_value = MOTION_read_controller_status_reg(handle);
	MOTION_set_controller_status_reg(handle,status_value);

	// clear HCTL-1100 controller mode flags F0,F3 and F5
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x00);							// Clear F0
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x03);							// Clear F3
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x04);							// Clear F4
	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x05);							// Clear F5
}



void MOTION_enter_idle_mode(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x05);							// Program counter register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x01);							// Enter idle mode
}



void MOTION_enter_align_mode(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x05);                    // Program counter register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x02);							// Enter align mode
}



void MOTION_enter_position_mode(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x05);				 			// Program counter register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x03);						  	// Enter control mode
}



void MOTION_execute_trapezoidal_move(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x08);							// Set F0
}



void MOTION_execute_proportional_velocity(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x0B);							// Set F3
}



void MOTION_end_proportional_velocity(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x03);							// Clear F3
}



void MOTION_execute_integral_velocity(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x0D);							// Set F5
}



void MOTION_end_integral_velocity(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr,0x00);							// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1,0x05);							// Clear F5
}



char MOTION_wait_until_move_complete(int handle)
{
char status;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	status = inportb(baseaddr+2);
	while ((status&0x20)==0x20)
	{
		// in profile control
		status = inportb(baseaddr+2);					// AIM104-MOTION-1 status reg
	}

	return 0;		// no error
}



void MOTION_set_command_position(int handle,
												unsigned long int command_position)
{
unsigned char command_MSB, command_NSB, command_LSB;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	command_MSB = (unsigned char) (command_position / 65536L);
	command_NSB = (unsigned char) ((command_position
											  - (command_MSB*65536L)) / 256L);
	command_LSB = (unsigned char) (command_position
											  - ((command_MSB*65536L)+(command_NSB*256L)));

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0C);						// command MSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, command_MSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0D);						// command NSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, command_NSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0E);						// command LSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, command_LSB);
}



void MOTION_set_final_position(int handle,
											unsigned long int final_position)
{
unsigned char final_MSB, final_NSB, final_LSB;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	final_MSB = (unsigned char) (final_position / 65536L);
	final_NSB = (unsigned char) ((final_position
												- (final_MSB*65536L)) / 256L);
	final_LSB = (unsigned char) (final_position
												- ((final_MSB*65536L)+(final_NSB*256L)));

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x29);						// final MSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, final_LSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x2A);						// final NSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, final_NSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x2B);						// final LSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, final_MSB);
}



void MOTION_preset_actual_position(int handle,
												unsigned long int actual_position)
{
unsigned char actual_MSB, actual_NSB, actual_LSB;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	actual_MSB = (unsigned char) (actual_position / 65536L);
	actual_NSB = (unsigned char) ((actual_position
												- (actual_MSB*65536L)) / 256L);
	actual_LSB = (unsigned char) (actual_position
												- ((actual_MSB*65536L)+(actual_NSB*256L)));

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x15);						// preset actual MSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, actual_MSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x16);						// preset actual NSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, actual_NSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x17);						// preset actual LSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, actual_LSB);
}



void MOTION_set_acceleration(int handle, int accel)
{
unsigned char accel_MSB;
unsigned char accel_LSB;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	accel_MSB = (unsigned char) (accel / 256L);
	accel_LSB = (unsigned char) (accel - (accel_MSB*256L));

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x26);						// accel LSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, accel_LSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x27);						// accel MSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, accel_MSB);
}



void MOTION_set_max_velocity(int handle, char velocity)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x28);						// max. velocity register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, velocity);
}



void MOTION_set_proportional_velocity(int handle, int velocity)
{
unsigned char vel_LSB;
unsigned char vel_MSB;
unsigned short baseaddr;
unsigned int u_velocity;

	baseaddr = boardstore[handle].base_addr;

	u_velocity = velocity;

	vel_MSB = (unsigned char) (u_velocity / 256L);
	vel_LSB = (unsigned char) (u_velocity - (vel_MSB*256L));

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x23);						// proportional vel LSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, vel_LSB);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x24);						// proportional vel MSB register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, vel_MSB);
}



void MOTION_set_integral_velocity(int handle, unsigned char velocity)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x3C);						// Integral velocity register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, velocity);
}



void MOTION_program_commutator(int handle,char velocity_timer,
						char ring,char X,char Y,char offset,char max_phase_advance)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x18);						// Ring register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, ring);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x19);						// velocity timer, phase advance
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, velocity_timer);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x1A);						// X register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, X);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x1B);						// Y (phase overlap) register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, Y);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x1C);						// Offset register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, offset);
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x1F);						// Max. phase advance register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, max_phase_advance);
}


char MOTION_read_board_status_reg(int handle)
{
char status_value;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	status_value = inportb(baseaddr+2);
	return(status_value);
}



char MOTION_read_flag_register(int handle)
{
char flags;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x00);						// Flag register
	inportb(baseaddr);                       // dummy read
	MOTION_wait_until_ready(handle);
	flags = inportb(baseaddr+1);
	return(flags);
}



unsigned long int MOTION_read_command_position(int handle)
{
unsigned char command_MSB, command_NSB, command_LSB;
unsigned long int command_position;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0E);						// command position LSB address
	inportb(baseaddr);                       // dummy read
	MOTION_wait_until_ready(handle);
	command_LSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0D);						// command position NSB address
	inportb(baseaddr);                       // dummy read
	MOTION_wait_until_ready(handle);
	command_NSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0C);						// command position MSB address
	inportb(baseaddr);								// dummy read
	MOTION_wait_until_ready(handle);
	command_MSB = inportb(baseaddr+1);

	command_position = (command_MSB*65536L) + (command_NSB*256L) + command_LSB;
	return(command_position);
}



unsigned long int MOTION_read_final_position(int handle)
{
unsigned char final_MSB, final_NSB, final_LSB;
unsigned long int final_position;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x29);						// final position LSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	final_LSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x2A);						// final position NSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	final_NSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x2B);						// final position MSB address
	inportb(baseaddr);								// dummy read
	MOTION_wait_until_ready(handle);
	final_MSB = inportb(baseaddr+1);

	final_position = (final_MSB*65536L) + (final_NSB*256L) + final_LSB;
	return(final_position);
}



unsigned long int MOTION_read_actual_position(int handle)
{
unsigned char actual_MSB, actual_NSB, actual_LSB;
unsigned long int actual_position;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x14);						// actual position LSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	actual_LSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x13);						// actual position NSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	actual_NSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x12);						// actual position MSB address
	inportb(baseaddr);								// dummy read
	MOTION_wait_until_ready(handle);
	actual_MSB = inportb(baseaddr+1);

	actual_position = (actual_MSB*65536L) + (actual_NSB*256L) + actual_LSB;
	return(actual_position);
}



int MOTION_read_actual_velocity(int handle)
{															// Proportional velocity mode only
unsigned char vel_MSB, vel_LSB;
int actual_velocity;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x34);					  	// actual velocity LSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	vel_LSB = inportb(baseaddr+1);

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x35);					  	// actual velocity MSB address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	vel_MSB = inportb(baseaddr+1);

	actual_velocity = vel_LSB + (256*vel_MSB);
	return(actual_velocity);
}


unsigned char MOTION_read_sample_timer(int handle)
{
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x0F);						// sample timer address
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	return(inportb(baseaddr+1));
}


char MOTION_open_loop_half_stepper(int handle,int total_steps,
																				int step_period)
{
unsigned char counts_per_step;
unsigned char scratch;
char phase_count = 0;
int step_count = 0;
int offset_value;
unsigned short baseaddr;

	baseaddr = boardstore[handle].base_addr;

	// This function assumes that the commutator has been programmed for half
	// step (X register = Y register) and that it is a 4 phase stepper motor.
	// step period = number of 8us counts (i.e. sample timer ticks) between steps

	// Read X-register to find how many (quad)counts per half-step
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x1A);						// X register
	inportb(baseaddr);                       	// dummy read
	MOTION_wait_until_ready(handle);
	counts_per_step = inportb(baseaddr+1);

	// Set the 'Hold commutator flag'
	MOTION_wait_until_ready(handle);
	outportb(baseaddr, 0x00);						// Flag register
	MOTION_wait_until_ready(handle);
	outportb(baseaddr+1, 0x0C);

	//Program sample timer
	MOTION_program_sample_timer(handle,step_period-1);

	if (total_steps>=0)
	{
		while(step_count < total_steps)
		{
			scratch = MOTION_read_sample_timer(handle);
			while (scratch<=20)
			{
				scratch = MOTION_read_sample_timer(handle);
			}
			if (phase_count>=8) phase_count=0;
			offset_value = phase_count * counts_per_step;
			MOTION_wait_until_ready(handle);
			outportb(baseaddr, 0x1C);						// Offset register
			MOTION_wait_until_ready(handle);
			outportb(baseaddr+1, offset_value);
			phase_count++;
			step_count++;
			scratch = MOTION_read_sample_timer(handle);
			while (scratch>20)
			{
				scratch = MOTION_read_sample_timer(handle);
			}
		}
	}
	else		// step in opposite direction
	{
		total_steps = -total_steps;
		phase_count=7;
		while(step_count < total_steps)
		{
			scratch = MOTION_read_sample_timer(handle);
			while (scratch<=20)
			{
				scratch = MOTION_read_sample_timer(handle);
			}
			if (phase_count<0) phase_count=7;
			offset_value = phase_count * counts_per_step;
			MOTION_wait_until_ready(handle);
			outportb(baseaddr, 0x1C);						// Offset register
			MOTION_wait_until_ready(handle);
			outportb(baseaddr+1, offset_value);
			phase_count--;
			step_count++;
			scratch = MOTION_read_sample_timer(handle);
			while (scratch>20)
			{
				scratch = MOTION_read_sample_timer(handle);
			}
		}
	}

	return(1);
}


#endif
