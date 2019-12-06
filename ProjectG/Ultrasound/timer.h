#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"

// Initializes Timer 2 for Ultrasound Capture
void InitTimerForUltrasound(void);

// Checks if the Capture Event flag is set. 
uint8_t IsCaptureEventFlagSet(void);

// Gets the time captured by the Capture Event flag.
uint32_t GetCaptureEventTimer(void);

// Initializes Timer 3 for time comparison
void InitTimer3(void);

// Waits
void WaitTIM3(uint32_t time_in_us);

uint32_t GetTIM3Timer(void);

#endif
