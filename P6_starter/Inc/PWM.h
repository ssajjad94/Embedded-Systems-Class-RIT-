#ifndef PWM_H
#define PWM_H

#include "stm32l476xx.h"

void InitPWM(void);

#define PWM_PRESCALER						7999
#define PWM_PERIOD							200

#define PWM_OFFSET							3				
	// Because some servo boxes are different, things may need to shift a bit

#define PWM_SCALE								 (5)
#define PWM_WIDTH_0							(PWM_SCALE*4	+ PWM_OFFSET)
#define PWM_WIDTH_1							(PWM_SCALE*7 + PWM_OFFSET)
#define PWM_WIDTH_2							(PWM_SCALE*10 + PWM_OFFSET)
#define PWM_WIDTH_3							(PWM_SCALE*13 + PWM_OFFSET)
#define PWM_WIDTH_4							(PWM_SCALE*16 + PWM_OFFSET)
#define PWM_WIDTH_5							(PWM_SCALE*20 + PWM_OFFSET)

#define PWM_DEFAULT_POSITION		PWM_WIDTH_0

#define PWM_MINIMUM							PWM_WIDTH_0
#define PWM_MAXIMUM							PWM_WIDTH_5

#define MIN_CHANGE							(3*PWM_SCALE)
#define MAX_CHANGE							(20*PWM_SCALE)

void SetPWMPulsePosition(uint8_t servo, uint8_t position);

void ShiftPWMLeft(uint8_t servo);

void ShiftPWMRight(uint8_t servo);

void InitTimerForPWM(void);
void InitGPIOForPWM(void);

// Helper functions
void SetPWMPulseWidth1(uint16_t pulse_width);
void SetPWMPulseWidth2(uint16_t pulse_width);

#endif
