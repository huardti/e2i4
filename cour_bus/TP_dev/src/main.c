/* Polytech Grenoble */
/* IESE/E2i */
/* TP SPI */
/********************* INCLUDES *********************************/
#include "stm32f4xx.h"

#include "Adc.h"
#include "code_eleve.h"
#include "Clocks.h"
#include "Port_conf.h"
#include "Spi.h"
#include "Timer.h"


/********************** VARIABLES *********************************************************************/
unsigned int adc_value;
unsigned int inter_value;

/*************************** CODE **********************************************************************/
int main(void)
{
	//Init clock and flash
	SystemInit();

	// Setting the clock's source and setting the prescaler of HCLK, PCLK1 & PCLK2
	ClockConfiguration();

	SetClocks(); 			// set peripherals clocks

	InitPeripheral();		// Init peripheral I/O's

	InitAdc();				//init ADC configuration

	InitTimer_1();			//init timer 1

//	InitInterrupt();		//init interrupt handlers

	Spi1_Init();			//init SPI mode

	while(1)
	{
		WriteToDAC(0x7FF);	//demi-�chelle

		//SignalTriangle();

		//RestitutionAnalogue();
	}
}
