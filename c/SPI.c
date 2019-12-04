#include "SPI.h"
#include "LCD.h"


/*****************************************************************************
 * SPI config & communication functions
 *****************************************************************************/
 
/* Init & configuration */
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

  SPI1->CR2 |= 0x1 << 12;   // Set FIFO reception threshold to 1/4 (when the FIFO holds less than 8-bits)
	SPI1->CR1 |= SPI_CR1_SPE; // Enable SPI1
}


/* Function to send 6 characters over SPI */
void sendSix(uint8_t outBuf[]) {
	uint8_t i;

	while (!(SPI1->SR & SPI_SR_TXE)) {;}
	*(uint8_t*)(&SPI1->DR) = 0x00; // Dummy byte to initialize clock
	
	for (i = 0; i < 6; i ++) {
		while (!(SPI1->SR & SPI_SR_TXE)) {;} // If the TX buffer is not empty just hang
		*(uint8_t*)(&SPI1->DR) = outBuf[i];  // Do only an 8-bit write access
	} 
}


/* Function to receive 6 characters over SPI and write the results to "str" */
void readSix(uint8_t inBuf[]) {
	uint8_t i;

	while (!(SPI1->SR & SPI_SR_RXNE)) {;} // Just hang while the buffer is empty (0x0)
	inBuf[0] = *(uint8_t*)(&SPI1->DR);    // Dummy bit will be overwritten
	
	for (i = 0; i < 6; i ++) {
  	while (!(SPI1->SR & SPI_SR_RXNE)) {;} // Just hang while the buffer is empty (0x0)
		inBuf[i] = *(uint8_t*)(&SPI1->DR);	  // 8-bit read access
	}
}


/*****************************************************************************
 * Joystick interrupt config and handlers
 *****************************************************************************/
 
/* NVIC and GPIO config */
void EXT_Initialize(void) {
	//NVIC->ISER[0] |= 0x100; //0001,0000,0000
	
	/* DEBUG CODE */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~(0x3 << 4);
	GPIOB->MODER |=  0x1 << 4;
	/* END DEBUG CODE */
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;  // Enable clock to GPIO port A
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable clock to SYSCFG
	
	GPIOA->MODER &= ~(0x33 << 6); // Set to pins 3 and 5 to input mode (0x00)
	GPIOA->PUPDR |=   0x22 << 6;  // Pull-DOWN pins 3 and 5

	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3_PA; //Set external interrupt #3 (pin 3) to port A (0x00)
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_PA; //set external interrupt #5 (pin 5) to port A (0x00)
	
	EXTI->FTSR1 |= EXTI_FTSR1_FT3; // Line 3 select falling edge trigger
	EXTI->FTSR1 |= EXTI_FTSR1_FT5; // Line 5 select falling edge trigger
	
	EXTI->IMR1 |= EXTI_IMR1_IM3; // Line 3 interrupt enable
	EXTI->IMR1 |= EXTI_IMR1_IM5; // Line 5 interrupt enable
	
	NVIC_EnableIRQ(EXTI3_IRQn);   // Enable interrupt for line 3
	NVIC_EnableIRQ(EXTI9_5_IRQn); // Enable interrupt for lines 9-5
}


void EXTI9_5_IRQHandler(void) {
  if ((EXTI->PR1 & EXTI_PR1_PIF5) == EXTI_PR1_PIF5) {
				
		if (scrollDelay < SCROLL_DELAY_MAX)
			scrollDelay += DELAY_STEP;
		
		EXTI->PR1 |= EXTI_PR1_PIF5; // Clear pending
  }
}


void EXTI3_IRQHandler(void) {
	if ((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		
	if (scrollDelay > SCROLL_DELAY_MIN)
			scrollDelay -= DELAY_STEP;        // Shorten the period of the delay
	
		EXTI->PR1 |= EXTI_PR1_PIF3; // Clear pending
	}	
}


/*****************************************************************************
 * String manipulation stuff
 *****************************************************************************/

/* Simple delay function */
void delay(void) {
  unsigned long count;
  for(count = 0; count < scrollDelay; count ++) {;} 
}


/* Function to "count" the non-null characters of a string */
uint8_t strLen(uint8_t str[]) {
	uint8_t i = 0;
	while (str[i])
		i ++;
  return i;
}


/* "Scroll" a string and fill a buffer with a 6-char slice of it*/
void movingString(uint8_t str[]) {
	static const uint8_t OFFSET = 6;
	uint8_t len;
  uint8_t i;
  uint8_t	j;
	uint8_t sendBuf[6];    // The string that will be sent to the slave
	uint8_t displayBuf[6]; // The string that will be displayed on the master
	
  len = strLen(str);

  for (i = 0; i < len; i++) {

    for (j = 0; j < 6; j++) {
      if (i + j < len)
        displayBuf[j] = str[i + j];
      else
        displayBuf[j] = str[i + j - len];
			
      if (i + j + OFFSET < len)
        sendBuf[j] = str[i + j + OFFSET];
      else
        sendBuf[j] = str[i + j + OFFSET - len];
		}
		
		sendSix(sendBuf);
		LCD_DisplayString(displayBuf);
		
    delay();
  }
}
