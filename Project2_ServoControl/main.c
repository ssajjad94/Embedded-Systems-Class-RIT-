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
	InitTimer3();
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
	servoTwoRecipe.servo = 2;
	
	// Load recipies
	LoadNewRecipe(&servoOneRecipe, recipie_test_3, sizeof(recipie_test_3) / sizeof(char));
	LoadNewRecipe(&servoTwoRecipe, recipie_test_3, sizeof(recipie_test_3) / sizeof(char));
	
	// Set them to paused initially
	TogglePauseRecipe(&servoOneRecipe, 1);
	TogglePauseRecipe(&servoTwoRecipe, 1);
	
	// Main execution loop
	uint8_t bRunning = 1;
	while (bRunning)
	{
		// Run recipe(s)
			// RunRecipe() will check if execution is paused or done.
		RunRecipe(&servoOneRecipe);
		RunRecipe(&servoTwoRecipe);
		
		// Check for errors in Servo One
		if (servoOneRecipe.m_NestedLoopError)
		{
			Green_LED_On();
			Red_LED_On();
		}
		else if (servoOneRecipe.m_CommandError)
		{
			Green_LED_Off();
			Red_LED_On();
		}
		else if (servoOneRecipe.m_bPaused)
		{
			Green_LED_Off();
			Red_LED_Off();
		}
		else if (servoOneRecipe.m_bRunningRecipe )
		{
			// If not paused or in any error state and is running:
			Green_LED_On();
			Red_LED_Off();
		}
		
		
		
		// Check for User Commands
		if (CheckForInputs())
		{
			char servoOneCommand = GetServoOneChar();
			char servoTwoCommand = GetServoTwoChar();
			ClearInputBuffer();
			
			// Perform commands
			PerformCommand(&servoOneRecipe, servoOneCommand);
			PerformCommand(&servoTwoRecipe, servoTwoCommand);
			
			PRINT("\n\r");
		}
		
		// Wait 100 ms.
			// (in 1 us)
		WaitTIM3(1000);
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


