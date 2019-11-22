#include "stm32l4xx.h"
#include <stdint.h>

void readSix(uint8_t* str) {
	uint8_t i;

	for (i = 0; i < 6; i++) {
  	while (!(SPI1->SR & SPI_SR_RXNE)) {;} // Just hang while the buffer is empty (0x0)
//    GPIOB->ODR |= 0x1 << 2;
		str[i] = *(uint8_t*)(&SPI1->DR);	  // 8-bit read access
//		GPIOB->ODR &= ~(0x1 << 2);
	}
	
}
