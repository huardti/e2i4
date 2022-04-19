/**************************** HEADER **************************************************************/
#include "stm32f4xx.h"

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
	SPI1->CR1 = 0x0B24;
	SPI1->CR2 = 0x0000;
	SPI1->CR1 |= 0x0040;		//enable SPI1
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



