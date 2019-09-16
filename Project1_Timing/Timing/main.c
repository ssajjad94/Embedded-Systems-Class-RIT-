#include "stm32l476xx.h"

#include <string.h>
#include <stdio.h>

#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "timer.h"
#include "GPIO.h"

#define DEBUG_MODE

char RxComByte = 0;
uint8_t buffer[BufferSize];

int main(void)
{
	uint16_t ul = 0;     // upper limit
	uint16_t ll = 0;     // lower limit
  
  /*
		Program initialization
  */
	
  System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	InitGPIO();
	InitTimer();
	
	/*
		Main program loop
	*/
	
	USART_Write (USART2, (uint8_t*)"Start of program.\n\r", BufferSize);
	while (1)
	{
//#ifdef DEBUG_MODE
		
			/*
				Debug Mode -> Sandbox to test timers, GPIO and other functionality.
				Preprocessor macro will ensure sandbox code is not compiled.
			*/
			
			
			while(1)
			{
				if (TIM2->SR & TIM_SR_CC1IF || 1)
				{
					uint16_t time = TIM2->CCR1;
					
					USART_WriteUInt16(USART2, time);
					USART_Write (USART2, (uint8_t*)"\t\t\t", BufferSize);
					
					uint16_t cnt = TIM2->CNT;
					
					USART_WriteUInt16(USART2, cnt);
					USART_Write (USART2, (uint8_t*)"\n\r", BufferSize);
				}
			}
			
			
			// USART_Write (USART2, (uint8_t*)"Upper Limit: 00\n\r", BufferSize);
			// USART_Write (USART2, (uint8_t*)"Lower Limit: 00\n\r", BufferSize);
		
//#else
		{
			// On startup, the program displays the upper and lower limits 
			//		and allows the user to either accept those values or to 
			//		change the lower limit to a new value.
			// The lower limit can be from 50 ms to 9950 ms. 
			// The upper limit will always be 100 ms longer than the lower limit
			
			char limitConfirmation = 0x00;
			
			do
			{
				char str[] = "Timer Project 1\n\nCurrent limits\r\n";
				
				USART_Write(USART2, (uint8_t *)str, strlen(str));
				USART_Write (USART2, (uint8_t*)"\nUpper Limit: 00\nLower Limit: 00\r\n",32);
			} 
			while (limitConfirmation != 'Y' || limitConfirmation != 'y');
			
			
			char rxByte = USART_Read(USART2);
			/*
			rxByte = USART_Read(USART2);
			if (rxByte == 'N' || rxByte == 'n'){
				Red_LED_Off();
				USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
			}
			else if (rxByte == 'Y' || rxByte == 'y')
			{
				Red_LED_On();
				USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
			}
			*/
		}
//#endif
	}
}

