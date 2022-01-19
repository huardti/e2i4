/**************************** HEADER **************************************************************/
#include "stm32f4xx.h"

#include "spi.h"


/**************************** CODE *****************************************************************/
// ***********************************************************************************************************************//
// Function  name  : Spi1_Init
// Description     : init spi ports
// Parameters      : N/A
// Return          : N/A
// ***********************************************************************************************************************//
void Spi1_Init(void)
{
	//PA5 et PA7 en alternate function mode
	GPIOA->MODER|=0x2<<5*2;
	GPIOA->MODER|=0x2<<7*2;
	//p.59
	GPIOA->AFR[0]=0x5<<5*4;
	GPIOA->AFR[1]=0x5<<7*4;
	//PB6 en GPIO
	GPIOB->MODER|=0x1<<6*2;
	//CONFIG transmit only
	SPI1->CR1=(0x0|SPI_CR_BIDIOE|SPI_CR_SPE);
}

// ***********************************************************************************************************************//
// Function  name  : Spi1Write
// Description     : Write 1 byte on SPI 1 port
// Parameters      : data to be written
// Return          : N/A
// ***********************************************************************************************************************//
void Spi1Write(unsigned int data)
{
	SPI1->DR=data;
}



