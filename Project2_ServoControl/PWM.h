#ifndef PWM_H
#define PWM_H

#include "stm32l476xx.h"

void InitPWM(void);

#define PWM_PRESCALER						7999
#define PWM_PERIOD							200
#define PWM_DEFAULT_WIDTH				100	

#define PWM_WIDTH_0							4
#define PWM_WIDTH_1							7
#define PWM_WIDTH_2							10
#define PWM_WIDTH_3							13
#define PWM_WIDTH_4							16
#define PWM_WIDTH_5							20

void SetPWMPulseWidth1(uint16_t pulse_width);
void SetPWMPulseWidth2(uint16_t pulse_width);

void SetPWMPulsePosition1(uint8_t position);
void SetPWMPulsePosition2(uint8_t position);

void InitTimerForPWM(void);
void InitGPIOForPWM(void);

#endif
