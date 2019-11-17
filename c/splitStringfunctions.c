uint8_t* string_split(uint8_t* myStringPointer)
{
	return myStringPointer + 6;
	
}

char get_char(uint8_t* newStringPointer, uint8_t* myStringPointer, int index, int stringLength, int currentLastIndex)
{
	if((currentLastIndex + 1) <= stringLength)
	{
		newStringPointer += index;
		return (char)*newStringPointer;
	}
	else if((currentLastIndex + 1) == stringLength + 1)
	{
		return ' ';
	}
	else
	{
		uint8_t* temp = myStringPointer + (currentLastIndex - stringLength);
		return (char)(*temp);
	}
}
