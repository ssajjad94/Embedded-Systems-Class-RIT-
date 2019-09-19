#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"

void InitTimer(void);
uint8_t IsCaptureEventFlagSet(void);
uint16_t GetCaptureEventTimer(void);

#endif
