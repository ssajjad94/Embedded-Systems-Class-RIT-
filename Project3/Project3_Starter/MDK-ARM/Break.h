#ifndef __BREAK_H    // header files should include guards
#define __BREAK_H

#include "stm32l4xx_hal.h"

typedef struct 
{
	uint32_t start_time;
	uint32_t end_time;
} Break;

#endif
