#include "stm32l476xx.h"

void InitTimer()
{
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM2->PSC = 79;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM2->EGR |= TIM_EGR_UG;
  
  // Turn off input capture 
  TIM2->CCER = 0;
  
  //
  TIM2->CCMR1
  
  // Enable inpute capture (on Capture/Compare 1)
  TIM2->CCER |= TIM_CCER_CC1E;
  
}