#include "main.h"

#define DEBUG_MODE


int main(void)
{
	/*
		Program initialization
  */
	
  System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	InitUltrasound();
	
	// Clear display
	PRINT("\033[2J");		// Clear terminal
	PRINT("\033[0;0H");	// Reset cursor to 0,0

	// Ultrasound program.
	PRINT("Ultrasound program.\n\r");
	
	
	
#ifdef DEBUG_MODE
			/*
				Debug Mode -> Sandbox to test timers, GPIO and other functionality.
				Preprocessor macro will ensure sandbox code is not compiled.
			*/
			{
				PRINT("DEBUGGING \n\n\r");
				
				while(1)
				{
					uint32_t time = GetUltrasoundMeasurement();
					PRINT_UINT32(time);
					PRINT("\n\r");
				}
				
				return 0;
			}
			
#else
	
			
			
	/*
		Power-On Self Test
	 */
	
	// When the program starts, execute a Power On Self Test (POST)
	// This test will confirm that the GPIO port is seeing pulses 
	//		at least once in 100 ms. 
	// If it fails, then over the option to try again.
	// If it succeeds, proceed to main program loop.
	
	PRINT("Performing Power On Self Test....\n\r");
	
	uint8_t bPOSTSuccessful = 1;
	
	if (bPOSTSuccessful == 0)
	{
		// Ask the user if they would like to continue anyway
		uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "The POST test failed. Would you like to continue anyway? (y/n):\t");
		
		// If yes, break. If no, prompt for a new value.
		if (confirmChar == 'n')
		{
			PRINT("Goodbye.\n\r\n\r");
			return -1;
		}
	}
	
	/*
		Main program loop
	*/

	uint8_t bRepeatMainProgram = 1;
	while (bRepeatMainProgram)
	{
		// Run the main program
		MainProgramLoop();
		
		// Prompt the user if they want to run again.
		uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "Would you like to run again? (y/n):\t");
		if (confirmChar == 'y' || confirmChar == 'Y')
			continue;
		else
			bRepeatMainProgram = 0;
	}
	
	PRINT("Goodbye.\n\r\n\r");
	return 0;
	
#endif
}


void MainProgramLoop()
{
}


void POSTTest()
{
}

