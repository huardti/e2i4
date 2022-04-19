/* Polytech Grenoble */
/* IESE/E2i */
/* TP SPI */


/********************************************************************************/
/*					HEADER FILES												*/
/********************************************************************************/
#include "stm32f446xx.h"

#include "Adc.h"
#include "Spi.h"
#include "Timer.h"

#include "code_eleve.h"

#include <stdio.h>
/********************************************************************************/
/*					STATIC														*/
/********************************************************************************/
static void Drive_CS_pin(char value);

/********************************************************************************/
/*					DEFINE														*/
/********************************************************************************/


/********************************************************************************/
/*					VARIABLES													*/
/********************************************************************************/
unsigned int count;

/********************************************************************************/
/*					SOURCE CODE													*/
/********************************************************************************/
/* timer par d�cr�ment d'une variable ou par utilisation du timer 1*/
void Timer_t1ms(unsigned int period)
{
	unsigned int i;

	for(i = 0; i < period; i++)
	{
		StartTimer_1();						//d�marre le timer

		while(GetUpdateEvent() == 0){;}		//contr�le si overflow du compteur (CNT == ARR)

		ResetUpdateEvent();					//reset du flag

		StopTimer_1();						//arr�t du compteur
	}
}




//---------------------------------------------------------------------------------------
static void Drive_CS_pin(char value)
{
	GPIOB->ODR = (GPIOB->ODR & ~(1<<6) ) | (value<<(6));
}

//----------------------------------------------------------------------------------------
void WriteToDAC(unsigned int value)
{
	value = value | ((0b0011)<<12); // 4 bits de configuration AB, BUF, GA, SHDN et 11 bits de data

	while(!(SPI1->SR & SPI_SR_TXE)); //On vérifie si TXE soit à 1

	Drive_CS_pin(0);
	Spi1Write(value);
	while(!(SPI1->SR & SPI_SR_RXNE)); //On vérifie si TXE soit à 1
	SPI1->DR;
	Drive_CS_pin(1);
}

//------------------------------------------------------------------------------------------
void SignalTriangle(void)
{
	WriteToDAC(count);
	count++;
	if(count > 0xFFF)
	{
		count = 0;
	}
}


//------------------------------------------------------------------------------------------
void RestitutionAnalogue(void)
{
	GPIOA->ODR  &= ~GPIO_ODR_ODR_1;
	uint16_t value;
	ADC_1_StartConversion();
	while(ADC_1_CheckEndOfConversion() == 0)
	{;
	}
	value = ADC_1_GetResult();
	ADC_1_ClearEndOfConversion();
	WriteToDAC(value);

	Timer_t1ms(9);
	GPIOA->ODR  |= GPIO_ODR_ODR_1;
	Timer_t1ms(1);
}


