/*
	main.c
	David Lin
*/


#include "main.h"

// #define DEBUG_MODE

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
					float distance = ConvertToMilimeters(time);
					
					USART_Printf("Time: %d us\tDistance: %f mm.\n\r", time, distance);
					
					WaitTIM3(1 * SECOND);
				}
				
				return 0;
			}
			
#else
	
			
			
	/*
		Power-On Self Test
	 */
	
	// When the program starts, execute a Power On Self Test (POST)
	// If it succeeds, proceed to main program loop.
	// If it fails, then offer the option to try again or proceed anyway.
	
	USART_Printf("Performing Power On Self Test....\n\r");
	uint8_t bPOSTSuccessful = POSTTest();
	USART_Printf("\n\r");
	if (bPOSTSuccessful == 0)
	{
		// Ask the user if they would like to continue anyway
		uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "The POST test failed. \n\rWould you like to continue to the main program anyway? (y/n):\t");
		
		// If yes, break. If no, prompt for a new value.
		if (confirmChar == 'n')
		{
			USART_Printf("Goodbye.\n\r\n\r");
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
		uint8_t confirmChar = PromptYesOrNo( (uint8_t *) "Would you like to run the program again? (y/n):\t");
		if (confirmChar == 'y' || confirmChar == 'Y')
			continue;
		else
			bRepeatMainProgram = 0;
	}
	
	USART_Printf("Goodbye.\n\r\n\r");
	return 0;
	
#endif
}


void MainProgramLoop()
{
	USART_Printf("\nStarting meausrement program...\n\r");
	
	// Perform the initial measurement (not stored)
	{
		uint32_t time = GetUltrasoundMeasurement();
		float distance = ConvertToMilimeters(time);
		
		USART_Printf("Distance: %f\n\r", distance);
	}
	
	// Collect up to 100 measurements
	float measurements[MEASUREMENT_CNT];
	float runningAvg = 1.0;
	for (int i = 0; i < MEASUREMENT_CNT; i++)
	{
		// Just to be safe, initialize all to zero.
		measurements[i] = 0.0;
	}
	uint32_t measurementCnt = 0;
	
	// Loop 
	while (measurementCnt < MEASUREMENT_CNT)
	{
		// Check for user interrupt
		if (USART_ReadAsync(USART2))
		{
			// Finish now.
			break;
		}
		
		// Perform the measurement
		uint32_t time = GetUltrasoundMeasurement();
		float distance = ConvertToMilimeters(time);
		
		// Check if the distance is within our limits. If so, save the measurement.
		if (distance > LOWER_RANGE_LIMIT && distance < UPPER_RANGE_LIMIT)
		{
			// USART_Printf("Distance: %f\n\r", distance);
			
			measurements[measurementCnt] = distance;
			measurementCnt += 1;
			
			// Update a running average of the measurements
			runningAvg = runningAvg * ((float) (measurementCnt - 1.0)) / ((float)(measurementCnt)) + distance * ( ((float) (1.0)) / ((float) measurementCnt));
			
			// USART_Printf("Running avg: %f\n\r", runningAvg);
		}
		
		// Wait so that only 10 measurements are made a second
		uint32_t wait_time = 100 * MILLISECOND;
		wait_time = wait_time - (SIGNAL_PERIOD + CYCLE_PERIOD + time);
		WaitTIM3(wait_time);
	}
	
	// Clean up -> try to find outliers
	if (measurementCnt == MEASUREMENT_CNT)
	{
		for (int i = 0; i < measurementCnt; i++)
		{
			float distance =  measurements[i];
			
			double perErr = (distance - runningAvg) / runningAvg;
			if (perErr < 0.0)
				perErr = perErr * (-1.0);
			
			// Check if we're in with 50% of the running average (get rid of baaaad outliers)
			while (perErr > 0.50)
			{
				// If not, re do that measurement.
				uint32_t time = GetUltrasoundMeasurement();
				distance = ConvertToMilimeters(time);
				measurements[i] = distance;
				
				// Update percent error
				perErr = (distance - runningAvg) / runningAvg;
				if (perErr < 0)
					perErr = perErr * (-1.0);
			}
		}
	}
	
	// Find the max and min
	float minDistance = measurements[0];
	float maxDistance = measurements[0];
	for (int i = 1; i < measurementCnt; i++)
	{
		if (measurements[i] < minDistance)
			minDistance = measurements[i];
		
		if (measurements[i] > maxDistance)
			maxDistance = measurements[i];
	}
	
	
	// Display back the measurements (as integers)
	for (int i = 0; i < measurementCnt; i++)
	{
		USART_Printf("%3d:\t %d mm\n\r", i, (int) measurements[i]);
	}
	
	// Display min and max
	USART_Printf("\n\r");
	USART_Printf("Min: %d mm\n\r", (int) minDistance);
	USART_Printf("Max: %d mm\n\r", (int) maxDistance);
	
}


uint8_t POSTTest()
{
	USART_Printf("Please place a large object about 100 mm from the sensor.\n\r");
	
	uint8_t bPOSTSuccess = 0;
	uint8_t bRunningPOST = 1;
	
	while (bRunningPOST)
	{
		// Prompt the user before running.
		PromptEnterToContinue((uint8_t *) "Once you object is in position, press <ENTER> to start the POST test.\n\r");
		
		// Run the ultrasound sensor.
		uint32_t distance = GetUltrasoundMeasurement();
		float distance_in_mm = ConvertToMilimeters(distance);
		
		if (distance < POST_VALID_RANGE_MIN)
		{
			// It's too close
			USART_Printf("The object is too close. Please place it about 100 mm from the sensor. \n\r");
			
			bPOSTSuccess = 0;
		}
		else if (distance > POST_VALID_RANGE_MAX)
		{
			// It's too far
			USART_Printf("The object is too far. Please place it about 100 mm from the sensor. \n\r");
			
			bPOSTSuccess = 0;
		}
		else
		{
			// It's just right.
			USART_Printf("Success! \n\r");
			
			bPOSTSuccess = 1;
			bRunningPOST = 0;
		}
		
		// If we failed, prompt the user to try again or exist with the failure.
		if (bPOSTSuccess == 0)
		{
			uint8_t bPromptRerun = PromptYesOrNo( (uint8_t *) "Would you like to try again? (y/n)" );
			if (bPromptRerun == 'y' || bPromptRerun == 'Y')
				bRunningPOST = 1;
			else
				bRunningPOST = 0;
		}
	}
	
	return bPOSTSuccess;
}


