#include "stm32l476xx.h"
#include "LCD.h"

void SPI_Init(void) {
  /* Enable clocks */
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
  
  GPIOE->MODER &= ~((uint32_t)0xFF << 24);   // Pins 12-15 clear 
  GPIOE->MODER |=   (uint32_t)0xAA << 24;    // Pins 12-15 AF mode

  GPIOE->OSPEEDR |= (uint32_t)0xFF << 24;    // Set output speed to "Very High"
  
  GPIOE->AFR[1] &= ~((uint32_t)0xFFFF << 16); // Clear pins 12-15 AFR
  GPIOE->AFR[1] |=   (uint32_t)0x5555 << 16;  // Set 12-15 to AF 5 (SPI)
  
  SPI1->CR1 |=   0x7 << 3; // Baud rate
  SPI1->CR1 |=   0x3; //set CPOL & CPHA  to 0
  SPI1->CR1 &= ~(0x1 << 10); //set RXONLY to 0 TRANSMIT/RECIEVE
  SPI1->CR1 &= ~(0x1 << 15); //set BIDIMODE to 0 DISABLED
  SPI1->CR1 |=   0x1 << 7; //set LSB to 1
  
  SPI1->CR1 &= ~(0x5 << 11); // Clear CRCL and CRCEN
  SPI1->CR1 &= ~(0x1 << 9);  // Disable SSM (software slave management)
  SPI1->CR1 |=   0x1 << 2;   // Set this board to Master (MSTR bit)
  
  SPI1->CR2 &= ~(0xF << 8); // Clear DS (data size)
  SPI1->CR2 |=   0x7 << 8;  // Set DS to 0111 (8-bit)
  
  // Ignore SSOE because we only have 1 master anyway
  
  SPI1->CR2 |= 0x1 << 12;  
}
