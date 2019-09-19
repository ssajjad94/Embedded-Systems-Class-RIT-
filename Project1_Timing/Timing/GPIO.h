#ifndef GPIO_H
#define GPIO_H

#include "stm32l476xx.h"

void InitGPIO(void);

// From PA0_Input_Verification
int read_pa0(void);

#endif
