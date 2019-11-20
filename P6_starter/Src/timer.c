#include "stm32l476xx.h"

#include "timer.h"


void InitTimer3()
{
	// We will be using TIM3
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM3->PSC = 7999;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM3->EGR |= TIM_EGR_UG;
  
	
	// Enable counter on TIM2 
	TIM3->CR1 |= TIM_CR1_CEN;
}

void WaitTIM3(uint32_t time_in_ms)
{
	uint32_t currTime = TIM3->CNT;
	
	while (TIM3->CNT - currTime < time_in_ms);
}
