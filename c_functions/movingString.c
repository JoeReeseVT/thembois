#include "LCD.h"
#include <stdio.h>
#include "stm32l476xx.h"

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
