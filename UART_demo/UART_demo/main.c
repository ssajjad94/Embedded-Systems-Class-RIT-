#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
uint16_t ul = 0;     //upper limit
uint16_t ll = 0;     //lower limit
char str[] = "Timer Project 1\n\nCurrent limits\r\n";

int main(void){
	char rxByte;
	int		a ;
	int		n ;
	int		i ;
	float b;
	
	System_Clock_Init(); // Switch System Clock = 80 MHz
  
  /*
  Call POST here
  */
  
	//LED_Init();
	UART2_Init();
		
	while (1){

		
		USART_Write(USART2, (uint8_t *)str, strlen(str));
    USART_Write (USART2, (uint8_t*)"\nUpper Limit: 00\nLower Limit: 00\r\n",32);
		rxByte = USART_Read(USART2);
		if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
		}
		else if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
		}
	}
}

