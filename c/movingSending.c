#include "LCD.h"
#include <stdio.h>
#include <stdint.h>
#include "stm32l476xx.h"

void delay(void) {
  unsigned long count;
  for(count = 0; count < 500000; count++) {} 
}

void movingString(uint8_t* str, uint8_t len) {
  unsigned int i, j;
  uint8_t buffer[6];
  uint8_t *mynewPointerTemp;
  int index = 0;
  char* myString[6];

  for (i = 0; i < len; i++) {

    for (j = 0; j < 6; j++) {
      if (i + j < len)
        buffer[j] = *(str + i + j);
      else
        buffer[j] = *(str + i + j - len);
    }
	
	mynewPointerTemp = string_split(str + i);
	for(int z = 0; z < 6; z++)
	{
		myString[z] = get_char(mynewPointerTemp, str, z, len);
	}
	
	sendSix(myString);
	
    LCD_DisplayString(&buffer[0]);
	
    delay();
  }
}



int baseIndexInString = 6; //global. says where the first index of the 2nd lcd will be


uint8_t* string_split(uint8_t* myStringPointer)
{
	return myStringPointer + 6;
	
}

//this will be for getting the character needed to be sent to the slave
char get_char(uint8_t* newStringPointer, uint8_t* myStringPointer, int index, int stringLength)
{
	
	int currentIndexInString = baseIndexInString + index;
	if(currentIndexInString < stringLength) //if we are below or at the last index
	{
		return (char)*(newStringPointer + index);
	}
	else if(currentIndexInString == stringLength) //just one over the length == space
	{
		return ' ';
	}
	else
	{
		//return the overflow character
		//example: cIIS is 2 over stringLength we should return myStringPointer (the first char
		uint8_t* temp = myStringPointer + (currentIndexInString - stringLength - 1);
		return (char)(*temp);
	}
}
