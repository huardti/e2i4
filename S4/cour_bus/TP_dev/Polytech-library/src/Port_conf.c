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
	//PA5 et PA7 en alternate function mode
	GPIOA->MODER|=0x2<<5*2;
	// GPIOA->MODER|=0x2<<6*2;
	GPIOA->MODER|=0x2<<7*2;
	//p.59
	GPIOA->AFR[0]|=0x5<< 5*4;
	// GPIOA->AFR[0]|=0x5<< 6*4;
	GPIOA->AFR[0]|=0x5<< 7*4;

	//PB6 en GPIO
	GPIOB->MODER|=0x1<<6*2;

	// configure potentiometer
	GPIOA->MODER |= 0x3 << 0*2;

	// periode d'echantillonage
	GPIOA->MODER|=0x1<<1*2;
	GPIOA->MODER|=0x1<<2*2;
}
