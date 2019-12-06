#include "stm32l476xx.h"

#include "timer.h"

/*
	TIM 2
*/

void InitTimerForUltrasound()
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
	
	// Program the input filter duration 		(from textbook)
	TIM2->CCMR1 &= ~TIM_CCMR1_IC1F;
	
  // Enable inpute capture (on Capture/Compare 1)
  //TIM2->CCER |= (1<<1 | 1<<3);
		//0x1;
		
	// Program the input prescaler (to 0)  (from textbook)
	TIM2->CCMR1 &= ~(TIM_CCMR1_IC1PSC);
	
	// Set to rising edge AND falling edge
	TIM2->CCER |= TIM_CCER_CC1NP;
	TIM2->CCER |= TIM_CCER_CC1P;
	
	// Enable capture of the counter				 (from textbook)
	TIM2->CCER |= TIM_CCER_CC1E;
	
	// Enable counter on TIM2 
	TIM2->CR1 |= TIM_CR1_CEN;
	
}
//

uint8_t IsCaptureEventFlagSet(void)
{
	// Checks if the Capture Event Flag is set
	
	if(TIM2->SR & TIM_SR_CC1IF)
		return 1;
	else
		return 0;
}
//


uint32_t GetCaptureEventTimer(void)
{
	// Gets the time set by the Capture Event Timer
	return TIM2->CCR1;
}
//



/*
	TIM 3
*/

void InitTimer3()
{
	// We will be using TIM3
	
  // Enable the clock for the selected timer RCC reg.
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
  
  // Load prescaler value into the TIMx->PSC reg.
  TIM3->PSC = 79;
  
  // Update new prescaler value using an update event, TIMx->EGR reg.
  TIM3->EGR |= TIM_EGR_UG;
  
	
	// Enable counter on TIM2 
	TIM3->CR1 |= TIM_CR1_CEN;
}
//

void WaitTIM3(uint32_t time_in_us)
{
	uint32_t timeToWait = time_in_us;
	
	while (timeToWait > 0)
	{
		uint32_t currTime = TIM3->CNT;
		
		if (timeToWait > 30000) // 65535
		{
			while (TIM3->CNT - currTime < 30000);
			timeToWait -= 30000;
		}
		else
		{
			while (TIM3->CNT - currTime < timeToWait);
			timeToWait = 0;
		}
	}
}
//

uint32_t GetTIM3Timer(void)
{
	// Gets the time set by the Capture Event Timer
	return TIM3->CNT;
}
//
