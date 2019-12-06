/*
	GPIO.c
	David Lin
*/


#include "GPIO.h"

// Private function prototypes
void InitGPIOForInputCapture(void);
void InitGPIOForOutput(void);


// Public functions

void InitGPIOForUltrasound()
{
	// Initialize GPIO Port A Pin 0 to use Alt Func (TIM2_CH1)
	InitGPIOForInputCapture();
	
	// Initialize GPIO Port D Pin 0 to use Output
	InitGPIOForOutput();
}


// Private functions


void InitGPIOForInputCapture()
{
	// Using GPIO Port A Pin 0 (TIM2_CH1)
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
  
  // Set MODER to use Alt function
  GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOA->MODER  |= GPIO_MODER_MODER0_1;
  
  // Set AFR (Alternate Function) to use AF1 
	//		PA0's AF1 is TIM2_CH1
	
  GPIOA->AFR[0] |= 0x1;
	GPIOA->AFR[1] |= 0x1;	// ? is this needed ?
}


void InitGPIOForOutput()
{
	// Using GPIO Port D Pin 0 
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	
	// Set MODER to use Output mode
	GPIOD->MODER &= ~GPIO_MODER_MODER0;
	GPIOD->MODER  |= GPIO_MODER_MODER0_0;
}




int read_pa0( void )
{
	// read the bottom bit of the GPIOA input lines which is PA0.
	// returns 1 if high, 0 if low.
	return GPIOA->IDR & 1 ;	
}
