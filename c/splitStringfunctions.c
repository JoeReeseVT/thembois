int CurrentIndexInString = 6; //global. Counts the index out of the whole string

uint8_t* string_split(uint8_t* myStringPointer)
{
	return myStringPointer + 6;
	
}

//this will be for getting the character needed to be sent to the slave
char get_char(uint8_t* newStringPointer, uint8_t* myStringPointer, int index, int stringLength)
{
	
	
	if((currentIndexInString + 1) <= stringLength) //if we are below or at the last index
	{
		currentIndexInString++;
		return (char)*(newStringPointer + index);
	}
	else if((currentIndexInString + 1) == stringLength + 1)
	{
		return ' ';
	}
	else
	{
		uint8_t* temp = myStringPointer + (currentLastIndex - stringLength);
		return (char)(*temp);
	}
}
