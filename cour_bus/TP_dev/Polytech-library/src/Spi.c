/**************************** HEADER **************************************************************/
#include "stm32f446xx.h"

#include "Spi.h"


/**************************** CODE *****************************************************************/
// ***********************************************************************************************************************//
// Function  name  : Spi1_Init
// Description     : init spi ports
// Parameters      : N/A
// Return          : N/A
// ***********************************************************************************************************************//
void Spi1_Init(void)
{
	SPI1->CR1 |= SPI_CR1_MSTR
	| SPI_CR1_BR_0
	| SPI_CR1_BR_1
	| SPI_CR1_BR_2
	| SPI_CR1_CPOL
	| SPI_CR1_CPHA
	| SPI_CR1_DFF
	| SPI_CR1_SSM
	| SPI_CR1_SSI
	| SPI_CR1_SPE;



}

// ***********************************************************************************************************************//
// Function  name  : Spi1Write
// Description     : Write 1 byte on SPI 1 port
// Parameters      : data to be written
// Return          : N/A
// ***********************************************************************************************************************//
void Spi1Write(unsigned int data)
{
	SPI1->DR = data;
}



