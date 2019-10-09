#include "PWM.h"

uint8_t PWMOneLastPosition = 0;
uint8_t PWMTwoLastPosition = 0;

// Helper function for setting a pulse in TIM2->CCR1
void SetPWMPulseWidth1(uint16_t pulse_width)
{
	TIM2->CCR1 = pulse_width;
}

// Helper function for setting a pulse in TIM2->CCR2
void SetPWMPulseWidth2(uint16_t pulse_width)
{
	TIM2->CCR2 = pulse_width;
}
	
void SetPWMPulsePosition(uint8_t servo, uint8_t position)
{
	// Helper function for translating position to pulse width.

	if (position <= 5)
	{
		uint16_t width = 0;
		
		// Set Last Position
		if (servo == 1)
			PWMOneLastPosition = position;
		else if (servo == 2)
			PWMTwoLastPosition = position;
		
		// Translate position to a width
		if (position == 0)
		{
			width = PWM_WIDTH_0;
		}
		else if (position == 1)
		{
			width = PWM_WIDTH_1;
		} 
		else if (position == 2)
		{
			width = PWM_WIDTH_2;
		} 
		else if (position == 3)
		{
			width = PWM_WIDTH_3;
		} 
		else if (position == 4)
		{
			width = PWM_WIDTH_4;
		} 
		else if (position == 5)
		{
			width = PWM_WIDTH_5;
		} 
		
		// Set the proper motor
		if (servo == 1)
			SetPWMPulseWidth1(width);
		else if (servo == 2)
			SetPWMPulseWidth2(width);
	}
}


void ShiftPWMLeft(uint8_t servo)
{
	uint8_t lastPosition = 0;
	if (servo == 1)
		lastPosition = PWMOneLastPosition;
	else if (servo == 2)
		lastPosition = PWMTwoLastPosition;
	
	if (lastPosition > 0)
		SetPWMPulsePosition(servo, lastPosition - 1);
	else
		SetPWMPulsePosition(servo, 0);
}

void ShiftPWMRight(uint8_t servo)
{
	uint8_t lastPosition = 0;
	if (servo == 1)
		lastPosition = PWMOneLastPosition;
	else if (servo == 2)
		lastPosition = PWMTwoLastPosition;
	
	if (lastPosition < 5)
		SetPWMPulsePosition(servo, lastPosition + 1);
	else
		SetPWMPulsePosition(servo, 5);
}

void InitPWM()
{
	// Init GPIO Port A, Pins 0 and 1 as PWM outputs
	InitGPIOForPWM();
	
	// Init Timer 2 as output with PWM
	InitTimerForPWM();
	
	// Initial positions
	SetPWMPulsePosition(1, PWM_DEFAULT_POSITION);
	SetPWMPulsePosition(2, PWM_DEFAULT_POSITION);
}

void InitTimerForPWM()
{
	// We will be using TIM2 Capture Channel 1 and 2 for PWM output onto a GPIO
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM2->PSC = PWM_PRESCALER;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM2->EGR |= TIM_EGR_UG;
  
  // Turn off / reset input capture 
  
  // Set up the CCMRx register for the desired capture channel (pg 906)
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;	// 00 -> CC1 channel is configured as output
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Output compare 2 preload enable
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	
	
	TIM2->CCMR1 &= ~TIM_CCMR1_CC2S;	// 00 -> CC2 channel is configured as output
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE; // Output compare 2 preload enable
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_2;
	
	// Enable the auto-Reload of the preload in the timer's control register
	TIM2->CR1 |= TIM_CR1_ARPE;
	
	// Enable the corresponding channel output bit in the timer's CCER register (pg 911)
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCER |= TIM_CCER_CC2E;
	
	// Force an update into the timer
  TIM2->EGR |= TIM_EGR_UG;
	
	// PWM signal info
	
	// Set the auto-reload register (which controls the period)
	TIM2->ARR = PWM_PERIOD;
	
	// Set the cpature/compare register 1 (which controls the pulse width)
	SetPWMPulsePosition(1, PWM_DEFAULT_POSITION);
	SetPWMPulsePosition(2, PWM_DEFAULT_POSITION);
	
	// Force an update into the timer
  TIM2->EGR |= TIM_EGR_UG;
	
	// Enable the timer
	TIM2->CR1 |= TIM_CR1_CEN;
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
	
	GPIOA->MODER &= ~GPIO_MODER_MODER1;
	GPIOA->MODER  |= GPIO_MODER_MODER1_1;
  
  // Set AFR (Alternate Function) to use AF1 
	//		PA0's AF1 is TIM2_CH1
	//		PA1's AF1 is TIM2_CH2
	
  GPIOA->AFR[0] |= 0x1;
	GPIOA->AFR[0] |= (0x1 << 4);
}

