/************************ HEADER FILES **************************************************************/
#include "stm32f4xx.h"

#include "Port_conf.h"

/************************ CODE *****************************************************************/

/*Configuration nécessaie=re
 * PA5: SPI-SCK
 * PA6: SPI-MISO
 * PA7: SPI-MOSI
 * PB6: SPI-CS/
*/


// ***********************************************************************************************************************//
// Function  name  : InitPeripheral
// Description     : Configures I/O port of the microcontroller
// Parameters      : N/A
// Return          : N/A
// ***********************************************************************************************************************//
void InitPeripheral(void)
{
	GPIOA->MODER |= 0xA800A803;
	/*
		PA15 | 	PA14 | 	PA13 | 	PA12 | 	PA11 | 	PA10 | 	PA9 | 	PA8 | 	PA7 | 	PA6 | 	PA5 |	PA4 | 	PA3 | 	PA2 | 	PA1 | 	PA0
		10	 10		10		00		00		00		00		00		10		10		10		00		00		00		00		00
	*/
	GPIOA->AFR[0] |= 0x55500000;
	GPIOA->AFR[1] |= 0x00000000;
	GPIOA->OTYPER |= 0x00000000;
	GPIOA->OSPEEDR |= 0x00000000;		
	GPIOA->PUPDR |= 0x00000000;
	
	GPIOB->MODER |= 0x00001000;
	/* 
		PB15 | 	PB14 | 	PB13 | 	PB12 | 	PB11 | 	PB10 | 	PB9 | 	PB8 | 	PB7 | 	PB6 | 	PB5 |	PB4 | 	PB3 | 	PB2 | 	PB1 | 	PB0
		00		00		00		00		00		00		00		00		00		01		00		00		00		00		00		00
	*/
	GPIOB->AFR[0] |= 0x00000000;
	GPIOB->AFR[1] |= 0x00000000;
	GPIOB->OTYPER |= 0x00000000;
	GPIOB->OSPEEDR |= 0x00000000;
	GPIOB->PUPDR |= 0x00000000;
}
