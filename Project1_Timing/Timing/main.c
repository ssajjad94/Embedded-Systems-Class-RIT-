#include "stm32l476xx.h"

#include <string.h>
#include <stdio.h>

#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "timer.h"
#include "GPIO.h"

#define PRINT(x)					USART_Write(USART2, (uint8_t*)x)
#define PRINT_UINT16(x)		USART_WriteUInt16(USART2, x);

//#define DEBUG_MODE

char RxComByte = 0;
uint8_t buffer[BufferSize];

int read_pa0( void )
{
	return GPIOA->IDR & 1 ;				// read the bottom bit of the GPIOA input lines which is PA0.
																// returns 1 if high, 0 if low.
}

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
			
	
			uint16_t prevCnt = 0;
			uint16_t nums[100];
			uint16_t numsCounted = 0;
			
			while(numsCounted < 100)
			{
				if (TIM2->SR & TIM_SR_CC1IF)
				{
					uint16_t time = TIM2->CCR1;
					
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
			
			
			// USART_Write (USART2, (uint8_t*)"Upper Limit: 00\n\r");
			// USART_Write (USART2, (uint8_t*)"Lower Limit: 00\n\r");
#endif
	
	/*
		Power-On Self Test
	 */
	
	// When the program starts, execute a Power On Self Test (POST)
	// This test will confirm that the GPIO port is seeing pulses 
	//		at least once in 100 ms. 
	// If it fails, then over the option to try again.
	// If it succeeds, proceed to main program loop.
	
	
	
	/*
		Main program loop
	*/
	
	uint16_t uLimit = 1050;     // upper limit
	uint16_t lLimit = 950;     // lower limit
	
	PRINT("\n\n\rStart of program.\n\r");
	while (1)
	{
		// On startup, the program displays the upper and lower limits 
		//		and allows the user to either accept those values or to 
		//		change the lower limit to a new value.
		// The lower limit can be from 50 ms to 9950 ms. 
		// The upper limit will always be 100 ms longer than the lower limit
		
		while(1) // Loop until the user confirms the limits.
		{
			// Print current limits
			PRINT("Upper Limit: ");
			PRINT_UINT16(uLimit);
			PRINT("\n\r");
			
			PRINT("Lower Limit: ");
			PRINT_UINT16(lLimit);
			PRINT("\n\r");
			
			// Ask the user to confirm or if they want to change the limits
			uint8_t confirmChar = 0;	// ConfirmChar keeps track of the user's response y/n
			while (1) 		// Loop until the user inputs a valid value
			{
				char rxByte = 0;
				
				PRINT("Would you like to continue using these values? (y/n):\t");
				while (rxByte != '\n' && rxByte != '\r')
				{
					rxByte = USART_Read(USART2);
					
					if(rxByte == 'y' || rxByte == 'Y' || rxByte == 'n' || rxByte == 'N')
						confirmChar = rxByte;
				} 
				PRINT("\n\r");
				
				// Check if we're looping: If there's a valid input, break out of the loop
				if(confirmChar != 0)
					break;
				else
					PRINT("Please input y or n\n\r");
			} 
			
			// If yes, break. If no, prompt for a new value.
			if (confirmChar == 'y' || confirmChar == 'Y')
			{
				break;
			}
			else
			{
				// Prompt the user to enter a new upper limit. 
				uint16_t limit = 0;
				while(1)	// Loop until the user inputs a valid limit.
				{
					char rxByte = 0;
					PRINT("Please enter a new lower limit. (50-9950)\t");
					limit = 0;
					rxByte = USART_Read(USART2);
					while (rxByte != '\n' && rxByte != '\r')
					{
						if (rxByte >= '0' && rxByte <= '9')
							limit = limit * 10 + (rxByte - '0');
						rxByte = USART_Read(USART2);
					} 
					
					// Check if we're looping: if there's a valid limit, break out of the loop
					if (limit < 50 || limit > 9950)
						PRINT("Upper Limit must be within the range of 50-9950\n\r");
					else
					{
						lLimit = limit;
						uLimit = lLimit + 100;
						break;
					}
				}
			}
			
			PRINT("\n\r");
		} 
		
		// At this point, we have the user-deisred upper limit and lower limit
		
		// Waits for the user to press the Enter key
		{
			PRINT("Press ENTER to begin....\n\r");
			char rxByte = 0;
			while (rxByte != '\n' && rxByte != '\r')
			{
				rxByte = USART_Read(USART2);
			}
		}
		
		// Initialize the empty buckets
		uint16_t buckets[101];
			// 0th index represents lLimit
			// 100th index represents uLimit
		for (int i = 0; i < 101; i++)
		{
			buckets[i] = 0;
		}
		
		// Perform the 1001 measurements (ignore the first 1 for 1000 measurements)s
		uint16_t measurementCnt = 0;
		uint16_t prevTime = 0;
		while(measurementCnt < 1001)
		{
			if (TIM2->SR & TIM_SR_CC1IF)
			{
				uint16_t time = TIM2->CCR1;
				uint16_t elapsedTime = time - prevTime;
				prevTime = time;
				
				if (measurementCnt > 0)
				{
					if (elapsedTime >= lLimit && elapsedTime <= uLimit)
					{
						uint16_t index = elapsedTime - lLimit;
						buckets[index] += 1;
					}
				}
				
				measurementCnt += 1;
			}
		}
		
		// After completing the 1000 measurements (1001 rising edges) the program
		//		displays ever non-zero bucketvia Putty. 
		for (int i = 0; i < 101; i++)
		{
			if (buckets[i] != 0)
			{
				uint16_t time = lLimit + i;
				PRINT_UINT16(time);
				PRINT("ms\t");
				
				PRINT_UINT16(buckets[i]);
				PRINT("\n\r");
			}
		}
		
		// Prompt the user if they want to run again.
	}
}

