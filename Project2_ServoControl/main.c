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
	LED_Init();
	
#ifdef DEBUG_MODE
	while (1)
	{
		char rxByte = 0;
		uint8_t limit = 0;
		
		// Try to read an input
		PRINT("Please enter a new position.\t");
		while (rxByte != '\n' && rxByte != '\r')
		{
			// If we have a digit value, increase the significance of the previous inputs by a digit
				// and append the current input's value.
			if (rxByte >= '0' && rxByte <= '5')
				limit = (rxByte - '0');
			
			rxByte = USART_Read(USART2);
		} 
		PRINT("\n\r");
		
		SetPWMPulsePosition1(limit);
	}
	
#endif

	/*
		Main Program Loop
	*/
	
	struct RecipeThread servoOneRecipe;
	servoOneRecipe.servo = 1;
	
	struct RecipeThread servoTwoRecipe;
	servoOneRecipe.servo = 2;
	
	uint8_t bRunning = 1;
	while (bRunning)
	{
		// 
		
		
		// Wait 100 ms. 
		
	}
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


