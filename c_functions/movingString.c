#ifndef LCD_H
  #include "LCD.h"
  #define LCD_H
#endif

#ifndef STDLIB_H
  #include <stdio.h>
  #define STDLIB_H
#endif

#ifndef STM32_H
  #include "stm32l476xx.h"
  #define STM32_H
#endif

#include "stm32l476xx.h"
#include "lcd.h"
#include <stdlib.h>

void delay(void) {
  unsigned long count;
  for(count = 0; count < 500000; count++) {} 
}

void movingString(uint8_t* str, uint8_t len) {
  unsigned int i, j;
  uint8_t buffer[6];
  for (i = 0; i < len; i++) {
    for (j = 0; j < 6; j++) {
      if (i + j < len)
        buffer[j] = *(str + i + j);
      else
        buffer[j] = *(str + i + j - len);
    }
    LCD_DisplayString(&buffer[0]);
    delay();
  }
}
