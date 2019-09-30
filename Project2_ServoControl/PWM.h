#ifndef PWM_H
#define PWM_H

#include "stm32l476xx.h"

void InitPWM(void);

#define PWM_PRESCALER		7999
#define PWM_PERIOD			200
#define PWM_	


void InitTimerForPWM(void);
void InitGPIOForPWM(void);

#endif
