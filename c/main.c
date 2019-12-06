#include "stm32l476xx.h"
#include "LCD.h"
#include "SPI.h"

#define IS_MASTER() 0

int main(void) {
	
	#if IS_MASTER() // MASTER CODE
	uint8_t testStr[] = "123456789ABCD \0";

	SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)
	LCD_Initialization();  //
  EXT_Initialize();
	
	while(1)
		movingString(testStr);
	
	
	#else // SLAVE CODE
	uint8_t inStr[6];

	SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)
	LCD_Initialization();
	
	while(1) {
		readSix(inStr);
		LCD_DisplayString(inStr);
	}
	#endif
	
}
