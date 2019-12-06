#ifndef SPI_H
#define SPI_H

#include "stm32l476xx.h"
#include <stdbool.h>
#include <stdint.h>

/* Global constants for scroll speed stuff */
static const uint32_t DELAY_STEP = 50000;
static const uint32_t SCROLL_DELAY_MIN = DELAY_STEP;
static const uint32_t SCROLL_DELAY_MAX = 15 * DELAY_STEP;

/* Global variable for speed control (to be modified by interrupts) */
static volatile uint32_t scrollDelay = 500000;

/* SPI communication functions */
void SPI_Init(bool isMaster);
void sendSix(uint8_t* str);
void readSix(uint8_t* str);

/* Joystic interrupt functions */
void EXT_Initialize(void);
void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

/* String scrolling */
uint8_t strLen(uint8_t* str);
void movingString(uint8_t* str);

/* Simple delay function */
void delay(void);

#endif
