#include "stm32l476xx.h"
#include "LCD.h"
#include "SPI.h"

#define IS_MASTER() 0

int main(void) {
	int i;//DEBUGGING

  RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER  &= ~(0x3 << 4); // Clear PB2 mode
	GPIOB->MODER  |=   0x1 << 4;  // Set PB2 to output mode
	
	#if IS_MASTER()
	  uint8_t* outStr = "This is a test";

	  SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)

//		for(i = 0; i < 100; i++) {;}
		
	  sendSix(outStr);
	  
	  return 0;
	#else
	  uint8_t inStr[] = "ABCDE";
	
	  SPI_Init(IS_MASTER()); // Initialize as master (1) or slave (0)
		
	  LCD_Initialization();
		
//	  while (!inStr[5])
	    readSix(&inStr[0]);
    
#if 1
		/* THIS FIXES A WEIRD ISSUE OF MISSING THE MSb OF THE TRANSMISSION */
    for (i = 5; i > 0; i--) {
		  inStr[i] >>= 1; // Shift the char right by 1
      inStr[i] |= (inStr[i - 1] & 0x1) << 7; // Set the char's MSb to that of the previous char's LSb
		}
		inStr[0] >>= 1;
#endif
		LCD_DisplayString(inStr);
		
		return 0;
	#endif
}
