#include "stm32l4xx.h"
#include <stdint.h>

void sendSix(uint8_t* str) {
	uint8_t i;
	uint8_t j;

	while (!(SPI1->SR & SPI_SR_TXE)) {;}
	*(uint8_t*)(&SPI1->DR) = 0x00; // Dummy byte to initialize clock
	
	for (i = 0; i < 6; i++) {
			
  	while (!(SPI1->SR & SPI_SR_TXE)) {;} // If the transmit buffer is not empty) then just hang
//    GPIOB->ODR |= 0x1 << 2;	
  	//for (j = 0; j < 100; j++);
    
		*(uint8_t*)(&SPI1->DR) = str[i];    // Do only an 8-bit write access
//		GPIOB->ODR &= ~(0x1 << 2);
	} 
//	GPIOB->ODR &= ~(0x1 << 2);
}
