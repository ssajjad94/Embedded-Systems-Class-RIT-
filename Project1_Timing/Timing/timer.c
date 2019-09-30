#include "stm32l476xx.h"

#include "timer.h"


void InitTimer()
{
	// We will be using TIM2 Capture Channel 1
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM2->PSC = 79; //79
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM2->EGR |= TIM_EGR_UG;
  
  // Turn off / reset input capture 
  TIM2->CCER = 0x0;
  
  // Set up the CCMRx register for the desired capture channel
	
	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
  TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;
	//TIM2->CCMR1 = 0x1;
	
	// Program the input filter duration 		(from textbook)
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F;
	
  // Enable inpute capture (on Capture/Compare 1)
  //TIM2->CCER |= (1<<1 | 1<<3);
		//0x1;
		
	// Program the input prescaler (to 0)  (from textbook)
	TIM2->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
	
	// Enable capture of the counter				 (from textbook)
	TIM2->CCER |= TIM_CCER_CC1E;
	
	// Enable related interrupts					 (from textbook)
	// TIM2->DIER |= TIM_DIER_CC1IE;
	
	// Enable DMA														 (from textbook)
	//TIM2->DIER |= TIM_DIER_CC1DE;
	
	// Enable counter on TIM2 
	TIM2->CR1 |= TIM_CR1_CEN;
	
}

uint8_t IsCaptureEventFlagSet(void)
{
	if(TIM2->SR & TIM_SR_CC1IF)
		return 1;
	else
		return 0;
}

uint16_t GetCaptureEventTimer(void)
{
	return TIM2->CCR1;
}
