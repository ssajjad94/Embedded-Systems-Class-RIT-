#include "main.h"

// #define DEBUG_MODE

int main(void)
{
	/*
		Program initialization
  */
	
  System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	InitGPIO();
	InitTimer();
	
	/*
	*/
#ifdef DEBUG_MODE
			/*
				Debug Mode -> Sandbox to test timers, GPIO and other functionality.
				Preprocessor macro will ensure sandbox code is not compiled.
			*/
			{
				PRINT("\n\n\r DEBUGGING \n\n\r");
				
				uint16_t prevCnt = 0;
				uint16_t nums[100];
				uint16_t numsCounted = 0;
				
				while(numsCounted < 100)
				{
					if (IsCaptureEventFlagSet())
					{
						uint16_t time = GetCaptureEventTimer();
						
	//					USART_WriteUInt16(USART2, time);
	//					USART_Write (USART2, (uint8_t*)"\t\t\t");
						
						
						uint16_t cnt = time ;
						uint16_t elapsedCnt = cnt - prevCnt;
						prevCnt = cnt;
						
						nums[numsCounted] = elapsedCnt;
						
						numsCounted += 1;
						
						//USART_WriteUInt16(USART2, elapsedCnt);
						//USART_Write (USART2, (uint8_t*)"\n\r");
					}
				}
				
				for (int i =0; i < 100; i++)
				{
					USART_WriteUInt16(USART2, nums[i]);
					USART_Write (USART2, (uint8_t*)"\n\r");
				}
				
				while(1){};
			
			}
#endif
	
	/*
		Power-On Self Test
	 */
	
	PRINT("\n\r");
	PRINT(HEADERLINE);
	
	PRINT("\t\tWelcome to the Project 1 - Timing program.\n\r");
	
	PRINT(HEADERLINE);
	PRINT("\n\r");
	
	
	// When the program starts, execute a Power On Self Test (POST)
	// This test will confirm that the GPIO port is seeing pulses 
	//		at least once in 100 ms. 
	// If it fails, then over the option to try again.
	// If it succeeds, proceed to main program loop.
	
	PRINT("Performing Power On Self Test....\n\r");
	
	uint8_t bPOSTSuccessful = PerformPOST();
	
	if (bPOSTSuccessful == 0)
	{
		// Ask the user if they would like to continue anyway
		uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "The POST test failed. Would you like to continue anyway? (y/n):\t");
		
		// If yes, break. If no, prompt for a new value.
		if (confirmChar == 'n')
		{
			PRINT("Goodbye.\n\r\n\r");
			return 1;
		}
	}
	
	/*
		Main program loop
	*/

	uint16_t uLimit = LOWER_LIMIT_DEFAULT + 100;     // upper limit
	uint16_t lLimit = LOWER_LIMIT_DEFAULT;     			 // lower limit
	
	// Loop for each iteration of program execute (the would you like to repeat at the end)
		// Loop ends when the user prompts to not repeat, exiting the program.
	uint8_t bRepeatMainProgram = 1;
	
	while (bRepeatMainProgram)
	{
		// On startup, the program displays the upper and lower limits 
		//		and allows the user to either accept those values or to 
		//		change the lower limit to a new value.
		// The lower limit can be from 50 ms to 9950 ms. 
		// The upper limit will always be 100 ms longer than the lower limit
		
		// Verify limits
		// Loop until the user confirms these limits.
		uint8_t bRepeatLimitVerificaton = 1;
		while(bRepeatLimitVerificaton) 
		{
			// Print current limits
			
			PRINT("\n\r");
			
			PRINT("Upper Limit: ");
			PRINT_UINT16(uLimit);
			PRINT("\n\r");
			
			PRINT("Lower Limit: ");
			PRINT_UINT16(lLimit);
			PRINT("\n\r");
			
			// Ask the user to confirm or if they want to change the limits
			uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "Would you like to continue using these values? (y/n):\t");
			
			// If yes, break. If no, prompt for a new value.
			if (confirmChar == 'y' || confirmChar == 'Y')
			{
				bRepeatLimitVerificaton = 0;
				break;
			}
			else
			{
				lLimit = PromptForLimit(LOWER_LIMIT_MIN, LOWER_LIMIT_MAX);
				uLimit = lLimit + 100;
			}
		} 
		
		// Waits for the user to press the Enter key
		{
			PRINT("Press ENTER to begin....");
			char rxByte = 0;
			while (rxByte != '\n' && rxByte != '\r')
			{
				rxByte = USART_Read(USART2);
			}
			PRINT("\n\r");
		}
		
		// Initialize the empty buckets
		uint16_t buckets[101];
			// 0th index represents lLimit
			// 100th index represents uLimit
		for (int i = 0; i < 101; i++)
		{
			buckets[i] = 0;
		}
		
		// Perform the 1001 measurements (ignore the first 1 for 1000 measurements)
		uint16_t measurementCnt = 0;
		uint16_t prevTime = 0;
		while(measurementCnt < 1001)
		{
			if (IsCaptureEventFlagSet())
			{
				// Find the elapsed time
				uint16_t time = GetCaptureEventTimer();
				uint16_t elapsedTime = time - prevTime;
				
				// Add it to the bucket (if applicable)
				if (measurementCnt > 0)
				{
					if (elapsedTime >= lLimit && elapsedTime <= uLimit)
					{
						uint16_t index = elapsedTime - lLimit;
						buckets[index] += 1;
					}
				}
				
				// Set values for iteration
				prevTime = time;
				measurementCnt += 1;
			}
		}
		
		// After completing the 1000 measurements (1001 rising edges) the program
		//		displays ever non-zero bucketvia Putty. 
		PRINT("\n\rHere are your buckets:\n\r");
		uint8_t bHasBucket = 0;
		for (int i = 0; i < 101; i++)
		{
			if (buckets[i] != 0)
			{
				bHasBucket = 1; 
				
				uint16_t time = lLimit + i;
				PRINT_UINT16(time);
				PRINT("ms\t");
				
				PRINT_UINT16(buckets[i]);
				PRINT("\n\r");
			}
		}
		if (bHasBucket == 0)
		{
			PRINT("\tThere are no non-empty buckets.\n\r");
		}
		PRINT("\n\r");
		
		// Prompt the user if they want to run again.
		{
			uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "Would you like to run again? (y/n):\t");
			
			// If yes, break. If no, prompt for a new value.
			if (confirmChar == 'y' || confirmChar == 'Y')
			{
				continue;
			}
			else
			{
				bRepeatMainProgram = 0;
			}
		}
	} // end of MainProgramLoop
	
	PRINT("Goodbye.\n\r\n\r");
	return 0;
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


/*
	Performs the POST test
*/
uint8_t PerformPOST(void)
{
	// Loop until the POST is done or abandoned
	uint8_t bPostTestSuccessful = 0;
	
	uint8_t bRepeatPostTest = 1;
	while(bRepeatPostTest)
	{
		uint32_t totalElapsedTime = 0;
		uint8_t bRunningPostTest = 1;
    
    uint16_t prevTime = 0;
    
		// Start the POST test
		while (bRunningPostTest == 1)
		{
			if (IsCaptureEventFlagSet())
			{
				bPostTestSuccessful = 1;
        PRINT("POST ran successfully!\n\r");
        return (bPostTestSuccessful);
			}
      else
      {
        // Calc time
        uint16_t elapsedTime = 0;
        uint16_t currTime = TIM2->CNT;
        if (prevTime != 0)
          elapsedTime = currTime - prevTime;
        prevTime = currTime;
        totalElapsedTime += elapsedTime;
        
        // If this was a failed case and we exceeded the total time,
        //		call this a failure.
        if (totalElapsedTime > POST_TIME_US)
        {
          bPostTestSuccessful = 0;
          bRunningPostTest = 0;
        }
      }
		}
		
		// If the POST failed, prompt the user to try again.
		if (bPostTestSuccessful == 0)
		{
			uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "Failed to verify POST. Would you like to try again? (y/n):\t");
			
			if (confirmChar == 'n' || confirmChar == 'N')
				bRepeatPostTest = 0;
		}
	}
	
	return bPostTestSuccessful;
}	



/*
 */
uint16_t PromptForLimit(uint16_t minLimit, uint16_t maxLimit)
{
	// Prompt the user to enter a new upper limit. 
	uint16_t limit = 0;
	
	// Loop until the user inputs a valid limit.
	while(limit < minLimit || limit > maxLimit)
	{
		// rxByte holds the read char within this scope.
		char rxByte = 0;
		limit = 0;
		
		// Try to read an input
		PRINT("Please enter a new lower limit. (");
		PRINT_UINT16(minLimit);
		PRINT("-");
		PRINT_UINT16(maxLimit);
		PRINT(")\t");
		while (rxByte != '\n' && rxByte != '\r')
		{
			// If we have a digit value, increase the significance of the previous inputs by a digit
				// and append the current input's value.
			if (rxByte >= '0' && rxByte <= '9')
				limit = limit * 10 + (rxByte - '0');
			
			rxByte = USART_Read(USART2);
		} 
		PRINT("\n\r");
		
		// Check if the number given is valid.
		if (limit < 50 || limit > 9950)
		{
			// If the limit is invalid, reiterate the condition and loop.
			PRINT("Lower Limit must be within the range of ");
			PRINT_UINT16(minLimit);
			PRINT("-");
			PRINT_UINT16(maxLimit);
			PRINT(".\n\r");
		}
	}
	
	return limit;
}

