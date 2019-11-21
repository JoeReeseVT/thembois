int baseIndexInString = 6; //global. says where the first index of the 2nd lcd will be

uint8_t* string_split(uint8_t* myStringPointer)
{
	return myStringPointer + 6;
	
}

//this will be for getting the character needed to be sent to the slave
char get_char(uint8_t* newStringPointer, uint8_t* myStringPointer, int index, int stringLength)
{
	
	int currentIndexInString = baseIndexInString + index;
	if((currentIndexInString + 1) <= stringLength) //if we are below or at the last index
	{
		return (char)*(newStringPointer + index);
	}
	else if((currentIndexInString + 1) == stringLength + 1 //just one over the length == space
	{
		return ' ';
	}
	else
	{
		//return the overflow character
		//example: cIIS is 2 over stringLength we should return myStringPointer (the first char
		uint8_t* temp = myStringPointer + (currentIndexInString - stringLength - 2);
		return (char)(*temp);
	}
}
