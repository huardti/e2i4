/* Polytech Grenoble */
/* IESE/E2i */
/* TP SPI */


/********************************************************************************/
/*					HEADER FILES												*/
/********************************************************************************/
#include "stm32f4xx.h"
#include "Adc.h"
#include "Spi.h"
#include "Timer.h"

#include "code_eleve.h"

/********************************************************************************/
/*					STATIC														*/
/********************************************************************************/
static void Timer_t100ms(unsigned int period);
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
/* timer par décrément d'une variable ou par utilisation du timer 1*/
static void Timer_t100ms(unsigned int period)/*timer 1ms*/
{
	unsigned int i;

	for(i = 0; i < period; i++)
	{
		StartTimer_1();						//démarre le timer

		while(GetUpdateEvent() == 0){;}		//contrôle si overflow du compteur (CNT == ARR)

		ResetUpdateEvent();					//reset du flag

		StopTimer_1();						//arrêt du compteur
	}
}




//---------------------------------------------------------------------------------------
static void Drive_CS_pin(char value)
{

	if(value == LOW)
	{
		GPIOB->ODR &= ~BIT6_MASK;
	}
	else
	{
		GPIOB->ODR |= BIT6_MASK;
	}
}

//----------------------------------------------------------------------------------------
void WriteToDAC(unsigned int value)
{
	//drive CS/ low
	while(!(SPI1->SR & SPI_I2S_FLAG_TXE));// wait TXE flag à 1
	Drive_CS_pin(LOW);

	//Timer_t100ms(1);

	value |= CONFIG_BITS_PORTA;

	//write to SPI MOSI
	Spi1Write(value);

	//Timer_t100ms(1);

	//drive CS/ high
	Drive_CS_pin(HIGH);

	//Timer_t100ms(5);
}


//------------------------------------------------------------------------------------------
void SignalTriangle(void)
{
	count++;

	WriteToDAC(count);

	if(count > 0xFFF)
	{
		count = 0;
	}
}


//------------------------------------------------------------------------------------------
void RestitutionAnalogue(void)
{
	ADC_1_StartConversion();

	while(ADC_1_CheckEndOfConversion() == 0){;}

	ADC_1_ClearEndOfConversion();
	Timer_t100ms(8);/*8ms*/
	WriteToDAC(ADC_1_GetResult());
}


