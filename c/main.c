#include "stm32l476xx.h"
#include "LCD.h"
#include "SPI.h"

int main(void) {
	uint16_t i;
	
	SPI_Init();

  while(1) {
		i = 1;
		for (i = 1; i != 0; i++);
		SPI1->DR = 'a';
	}
}
