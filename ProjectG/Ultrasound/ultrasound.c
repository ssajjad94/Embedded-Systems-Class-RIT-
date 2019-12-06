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
	
	// Wait for response 
	uint32_t startWaitTime = GetTIM3Timer();
	
	// Get the time of echo edges
	uint32_t startEchoTime = 0;
	uint32_t endEchoTime = 0;
	
	// Capture two data points (one after the other)
	uint8_t echoState = 0; 	// 0 for initial; 1 if collected startEchoTime; 2 if colelcted both
	while (echoState < 2)
	{
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
				echoState = 2;
			}
		}
	}
	
	// Return echo period
	uint32_t echoPeriod = endEchoTime - startEchoTime;
	
	return echoPeriod;
}


// Private functions


void GeneratePingSignal()
{
	// Set Port D Pin 0 high
	GPIOD->ODR |= GPIO_ODR_ODR_0;
	
	// Wait
	WaitTIM3(11);
	
	// Turn off 
	GPIOD->ODR &= ~GPIO_ODR_ODR_0;
}


