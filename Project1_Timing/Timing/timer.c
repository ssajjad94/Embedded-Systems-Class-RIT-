#include "stm32l476xx.h"

#include "timer.h"

void InitTimer()
{
	// We will be using TIM2 
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM2->PSC = 79;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM2->EGR |= TIM_EGR_UG;
  
  // Turn off / reset input capture 
  TIM2->CCER &= ~TIM_CCER_CC1E;
  
  // Set up the CCMRx register for the desired capture channel
  TIM2->CCMR1 = TIM_CCMR1_CC1S_0; 
			//TIM_CCMR1_CC1S;
  
  // Enable inpute capture (on Capture/Compare 1)
  TIM2->CCER |= TIM_CCER_CC1E;
	
	// Enable counter on TIM2 
	TIM2->CR1 |= TIM_CR1_CEN;
}
