#ifndef SPI_H
#define SPI_H

#include "stm32l476xx.h"

void SPI_Init(bool isMaster);
char* string_split(char* myStringPointer);
char get_char(char* newStringPointer, char* myStringPointer, int index, int stringLength);
void sendSix(uint8_t* str);
void readSix(uint8_t* str);

#endif
