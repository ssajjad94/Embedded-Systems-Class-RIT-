#include "main.h"

// #define DEBUG_MODE

int main(void)
{
	/*
		Program initialization
  */
	
  System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	InitPWM();
	
	/*
	
	*/
}


/*
	Prompts the user a given prompt, and loops until a y or n is returned.

	returns 'y' or 'n'
*/
uint8_t PromptYesOrNo(uint8_t* prompt)
{
	uint8_t retVal = 0;
	
	// Loop until the user inputs a valid value
	while (retVal == 0)
	{
		// rxByte holds the read char within this scope.
		char rxByte = 0;
		
		// Try to read an input
		PRINT(prompt);
		while (rxByte != '\n' && rxByte != '\r')
		{
			rxByte = USART_Read(USART2);
			
			if(rxByte == 'y' || rxByte == 'Y' || rxByte == 'n' || rxByte == 'N')
				retVal = rxByte;
		} 
		PRINT("\n\r");
	} 
	
	if (retVal == 'Y')
		retVal = 'y';
	
	if (retVal == 'N')
		retVal = 'n';
	
	return retVal;
}


