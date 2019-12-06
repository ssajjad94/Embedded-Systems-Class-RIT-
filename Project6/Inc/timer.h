#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"

void InitTimer3(void);

void WaitTIM3(uint32_t time_in_us);

#endif
