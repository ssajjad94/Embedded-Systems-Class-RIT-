#include "GPIO.h"


void InitGPIO()
{
  // Using GPIO PortA Pin 0
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
  
  // Set MODER to use Alt function
  GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->MODER  |= GPIO_MODER_MODER0_1;
  
  // Set AFR (Alternate Function) to use AF1 (TIM2_CH1)
  // GPIOA->AFR[1] |= GPIO_AFRH_AFRH1;
  GPIOA->AFR[0] |= 0x1;
}

int read_pa0( void )
{
	// read the bottom bit of the GPIOA input lines which is PA0.
	// returns 1 if high, 0 if low.
	return GPIOA->IDR & 1 ;	
}
