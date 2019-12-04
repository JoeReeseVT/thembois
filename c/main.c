#include "stm32l476xx.h"
#include "LCD.h"
#include "SPI.h"

#define IS_MASTER() 1

int main(void) {
	#if IS_MASTER()
	uint8_t testStr[] = "123456789ABCD\0";

	SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)
	LCD_Initialization();  //
  EXT_Initialize();
	
	while(1)
		movingString(testStr);
	
	#else
	uint8_t i;

	uint8_t inStr[6];

	SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)
	LCD_Initialization();
	
	while(1) {
		readSix(inStr);
		
		/* THIS FIXES A WEIRD ISSUE OF MISSING THE MSb OF THE TRANSMISSION */
		for (i = 5; i > 0; i--) {
			inStr[i] >>= 1; // Shift the char right by 1
			inStr[i] |= (inStr[i - 1] & 0x1) << 7; // Set the char's MSb to that of the previous char's LSb
		}
		inStr[0] >>= 1;
		
		LCD_DisplayString(inStr);
	}
	#endif
}
