/*
	ultrasound.c
	David Lin
*/


#include "ultrasound.h"

// Private prototypes
void GeneratePingSignal(void);


// Public functions

void InitUltrasound()
{
	// 
	InitGPIOForUltrasound();
	InitTimerForUltrasound();
	InitTimer3();
}

uint32_t GetUltrasoundMeasurement()
{
	// Generate 10us Initiate signal
	GeneratePingSignal();
	
	// Wait for the signal period (8 pulses at 40kHz)
	WaitTIM3(SIGNAL_PERIOD);
	
	// Measure the echo response
	uint32_t startEchoTime = 0;
	uint32_t endEchoTime = 0;
	uint32_t postEchoTime = 0;
	
	// Set up timeout variables
	uint32_t startWaitTime = GetTIM3Timer();
	uint32_t timeoutCount = 0;
	
	// Capture two data points (one after the other) -> three somtimes.
	uint8_t echoState = 0; 	
		// 0 for initial; 
		// 1 if collected startEchoTime; 
		// 2 if collected both and the data is within limits;
		// 3 if collected both and sometime looks wrong (usually happens in first call) and the data isn't within limits. Might need another interrupt.
		// 4 if in 3-case and collected final interrupt.
	// Exit when echoState is in state=2 or state =4, or timeout.
	while (timeoutCount < TIMOUT_MAX_ITER)
	{
		// Check for capture event
		if (IsCaptureEventFlagSet())
		{
			uint32_t time = GetCaptureEventTimer();
			if (echoState == 0)
			{
				startEchoTime = time;
				echoState = 1;
			}
			else if (echoState == 1)
			{
				endEchoTime = time;
				
				if (endEchoTime - startEchoTime > MAX_ECHO_TIME)
					echoState = 3;	// Oddball case. Keep looking for another interupt until timeout.
				else
					echoState = 2;  // Good data case.
			}
			else
			{
				postEchoTime = time;
				echoState = 4;
			}
			
			startWaitTime = GetTIM3Timer();
			timeoutCount = 0;
		}
		// Check for timeout
		else if (GetTIM3Timer() - startWaitTime > TIMEOUT_TIME_IN_US)
		{
			timeoutCount += 1;
			
			startWaitTime = GetTIM3Timer();
		}
		
		// Check for exit case
		if (echoState == 2 || echoState == 4)
			break;
	}
	
		// Wait cycle period
	WaitTIM3(30 * MILLISECOND);
	
	// Calc echoPeriod
	uint32_t echoPeriod = 0;
	
	if (echoState == 2)
	{
		// Return echo period
		echoPeriod = endEchoTime - startEchoTime;
	}
	else if (echoState == 4)
	{
		// This usually happens the first iteration somehow.
		
		// We got 3 interupts somehow. 
		//PRINT("Got 3 interrupts. Debug this.\n\r");
		uint32_t period_one = endEchoTime - startEchoTime;
		uint32_t period_two = postEchoTime - endEchoTime;
		//PRINT("PENGUIN YAY ONE \n\r");
		//PRINT("\t");
		//PRINT_UINT32(period_one);
		//PRINT("\t");
		//PRINT_UINT32(period_two);
		//PRINT("\n\r");
		
		if (period_one > period_two)
			echoPeriod = period_two;
		else
			echoPeriod = period_one;
	}
	else
	{
		// One or No interrupots. Return 0 for failure.
		echoPeriod = 0;
	}
	
	// Convert from time to distance (return in mm)
	
	return echoPeriod;
}


float ConvertToMilimeters(uint32_t distance_in_us)
{
	float distance = ((float) distance_in_us) * (10.0 / 58.0);
	
	return distance;
}


// Private functions


void GeneratePingSignal()
{
	// Set Port D Pin 0 high
	GPIOD->ODR |= GPIO_ODR_ODR_0;
	
	// Wait
	WaitTIM3(10);
	
	// Turn off 
	GPIOD->ODR &= ~GPIO_ODR_ODR_0;
}


