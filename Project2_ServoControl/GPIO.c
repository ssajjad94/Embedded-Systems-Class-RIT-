#include "GPIO.h"


void InitGPIO()
{
  // Using GPIO Port A Pin 0 (TIM2_CH1)
	// 	 and GPIO Port A Pin 1 (TIM2_CH2)
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
  
  // Set MODER to use Alt function
  GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->MODER  |= GPIO_MODER_MODER0_1;
  
  // Set AFR (Alternate Function) to use AF1 
	//		PA0's AF1 is TIM2_CH1
	//		PA1's AF1 is TIM2_CH2
	
  GPIOA->AFR[0] |= 0x1;
	GPIOA->AFR[1] |= 0x1;	// ? is this needed ?
}

int read_pa0( void )
{
	// read the bottom bit of the GPIOA input lines which is PA0.
	// returns 1 if high, 0 if low.
	return GPIOA->IDR & 1 ;	
}