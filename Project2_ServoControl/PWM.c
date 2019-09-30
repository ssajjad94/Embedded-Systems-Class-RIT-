#include "PWM.h"

void InitPWM()
{
	// Init GPIO Port A, Pins 0 and 1 as PWM outputs
	InitGPIOForPWM();
	
	// Init Timer 2 as output with PWM
	InitTimerForPWM();
	
	// 
}

void InitTimerForPWM()
{
	// We will be using TIM2 Capture Channel 1 and 2 for PWM output onto a GPIO
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM2->PSC = 7999;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM2->EGR |= TIM_EGR_UG;
  
  // Turn off / reset input capture 
  TIM2->CCER = 0x0;
  
  // Set up the CCMRx register for the desired capture channel (pg 906)
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;	// 00 -> CC1 channel is configured as output
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Output compare 2 preload enable
	TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;	// 00 -> CC2 channel is configured as output
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE; // Output compare 2 preload enable
	
	// Enable the auto-Reload of the preload in the timer's control register
	TIM2->CR1 |= TIM_CR1_ARPE;
	
	// Enable the corresponding channel output bit in the timer's CCER register (pg 911)
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCER |= TIM_CCER_CC2E;
	
	// Force an update into the timer
  TIM2->EGR |= TIM_EGR_UG;
	
	
	
	// PWM signal info
	
	// Set the auto-reload register
	TIM2->ARR = 200;
	
}


void InitGPIOForPWM()
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

