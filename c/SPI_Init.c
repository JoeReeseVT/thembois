#include "stm32l476xx.h"
#include "LCD.h"

void SPI_Init(bool isMaster) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // Enable clock to GPIOE
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;  // Enable clock to SPI1
  
  GPIOE->MODER &= ~((uint32_t)0xFF << 24);   // Pins 12-15 clear 
  GPIOE->MODER |=   (uint32_t)0xAA << 24;    // Pins 12-15 AF mode

  GPIOE->OSPEEDR &= ~((uint32_t)0xFF << 24); // Clear output speed
	GPIOE->OSPEEDR |=   (uint32_t)0xAA << 24;  // Set pins 12-15 output speed to 10 (high speed)
	
	GPIOE->PUPDR &= ~((uint32_t)0xFF << 24); // Clear the pull-up/down for pins 12-15
	GPIOE->PUPDR |=   (uint32_t)0x55 << 24;  // Set pins 12-15 to 01 (pull-up) 
  
  GPIOE->AFR[1] &= ~((uint32_t)0xFFFF << 16); // Clear pins 12-15 AFR
  GPIOE->AFR[1] |=   (uint32_t)0x5555 << 16;  // Set 12-15 to AF 5 (SPI)
  
	if (isMaster) {
		SPI1->CR1 &= ~(0x7 << 3);  // Clear baud rate
		SPI1->CR1 |=   0x4 << 3;   // Set baud rate
  }
		
  SPI1->CR1 |=  SPI_CR1_CPOL;        // Set CPOL to be "idle high" & CPHA  to "falling edge"
	SPI1->CR1 &= ~SPI_CR1_CPHA;
		
	if (isMaster)
		SPI1->CR1 &= ~(0x1 << 10); // Set RXONLY to 0 TRANSMIT/RECEIVE
  else
		SPI1->CR1 |=   0x1 << 10;
		
	SPI1->CR1 &= ~(0x1 << 15); // Set BIDIMODE to 0 DISABLED
  SPI1->CR1 |=   0x1 << 7;   // Set LSB to 1
  
  SPI1->CR1 &= ~(0x5 << 11); // Clear CRCL and CRCEN
  SPI1->CR1 |=   0x1 << 9;   // Enable SSM (software slave management)
	
	if (isMaster)
    SPI1->CR1 |=  SPI_CR1_MSTR;   // Set this board to Master
	else
	  SPI1->CR1 &= ~SPI_CR1_MSTR;   // ... Or don't
	
  SPI1->CR2 &= ~(0xF << 8); // Clear DS (data size)
  SPI1->CR2 |=   0x7 << 8;  // Set DS to 0111 (8-bit)
	
	if (isMaster)
		SPI1->CR2 |= SPI_CR2_SSOE; // Enable SS output

  SPI1->CR2 |= 0x1 << 12;  // Set FIFO reception threshold to 1/4 (when the FIFO holds less than 8-bits)
	
	SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI1
}
