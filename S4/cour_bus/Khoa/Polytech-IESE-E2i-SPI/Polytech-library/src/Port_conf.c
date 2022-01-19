/************************ HEADER FILES **************************************************************/
#include "stm32f4xx.h"

#include "Port_conf.h"

/************************ CODE *****************************************************************/

/*Configuration n�cessaie=re
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

	/* Configure PA4-PA10 as outputs, push-pull, high speed*/

	GPIOA->MODER |= 0b10<<10; //PA5

	GPIOA->MODER |= 0b10<<12; //PA6

	GPIOA->MODER |= 0b10<<14; //PA7

	GPIOB->MODER |= 1<<12;  //PB6

	GPIOA->OTYPER &=~ 1<<7;
	GPIOA->OTYPER &=~ 1<<6;
	GPIOB->OTYPER &=~ 1<<6;

	GPIOA->AFR[0] |= 5<<20;
	GPIOA->AFR[0] |= 5<<24;
	GPIOA->AFR[0] |= 5<<28;

	/*configure PA0 pour AN0*/
	GPIOA->MODER |= 0b11<<0; //PA0
}