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
//static void Timer_t1ms(unsigned int period);
static void Drive_CS_pin(char value);

/********************************************************************************/
/*					DEFINE														*/
/********************************************************************************/


/********************************************************************************/
/*					VARIABLES													*/
/********************************************************************************/
unsigned int count;
static void Timer_t100ms(unsigned int period);
static void Drive_CS_pin(char value);

static uint16_t TAMPON[81];
static int16_t COEFFS [] ={-8,-8,-8,-9,-9,-9,-9,-10,-10,-10,-10,-11,-11,-11,-11,-12,-12,-12,-12,-13,-13,-13,-13,-13,-13,-14,-14,-14,-14,-14,-14,-14,-14,-14,-15,-15,-15,-15,-15,-15,985,-15,-15,-15,-15,-15,-15,-14,-14,-14,-14,-14,-14,-14,-14,-14,-13,-13,-13,-13,-13,-13,-12,-12,-12,-12,-11,-11,-11,-11,-10,-10,-10,-10,-9,-9,-9,-9,-8,-8,-8};
/********************************************************************************/
/*					SOURCE CODE													*/
/********************************************************************************/
/* timer par décrément d'une variable ou par utilisation du timer 1*/
void Timer_t1ms(unsigned int period)
{
	unsigned int i;

	for(i = 0; i < period; i++)
	{
		StartTimer_1();						//démarre le timer

		while(GetUpdateEvent() == 0){;}		//contrôle si overflow du compteur (CNT == ARR)

		ResetUpdateEvent();					//reset du flag

		filtre();

		StopTimer_1();						//arrêt du compteur
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
	int lecture;
	value = value + ((0b0011)<<12); // 4 bits de configuration AB, BUF, GA, SHDN et 11 bits de data


	while(!(SPI1->SR & 0x0002)); //On vérifie si TXE soit libre donc à 1

	Drive_CS_pin(0);
	Spi1Write(value);


	while(!(SPI1->SR & 0x0001)); //On vérifie si RXNE soit libre donc à 1

	while(SPI1->SR & 0x0080);//On vérifie si bsy soit libre

	Drive_CS_pin(1);
	lecture = SPI1->DR;
}


//------------------------------------------------------------------------------------------
void SignalTriangle(void)
{
	// A compléter ...
	for (unsigned int value=0; value <= 0xFFF; value++)
	{
		WriteToDAC(value);
		Timer_t1ms(0.1);

	}
}


//------------------------------------------------------------------------------------------
void RestitutionAnalogue(void)
{

	// A compléter ...
	uint8_t check_flag=0;
	uint16_t valeur = 0;

	ADC_1_StartConversion();

	while((check_flag = ADC_1_CheckEndOfConversion()) != 0x2); // check fin de conversion flag

	ADC_1_ClearEndOfConversion();

	valeur = ADC_1_GetResult();

	WriteToDAC(valeur);


}

void RAZTAMP()
{
	for(int i=0; i < 81; i++)
	{
		TAMPON[i] = 0;
	}
}

void INSTAMP(uint16_t value)
{

	for(int i=0; i < 81; i++)
	{
		TAMPON[i] = TAMPON[i+1];
	}

	TAMPON[80] = value;
}

void filtre()
{
	uint8_t check_flag=0;
	uint16_t valeur = 0;
	int tmp = 0;
	int i=0;


	ADC_1_StartConversion();

	while((check_flag = ADC_1_CheckEndOfConversion()) != 0x2); // check fin de conversion flag

	ADC_1_ClearEndOfConversion();

	valeur = ADC_1_GetResult();

	INSTAMP(valeur);

	for(i=0; i<81; i++)
	{
		tmp = tmp + TAMPON[i] * COEFFS[i];

	}
	tmp = (tmp/1000) + 2048;
	WriteToDAC(tmp);

	//Timer_t1ms(7.8);

}

