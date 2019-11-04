#ifndef __CUSTOMER_H    // header files should include guards
#define __CUSTOMER_H

#include "stm32l4xx_hal.h"

typedef struct 
{
	uint32_t arrival_time;
	uint32_t service_start_time;
	uint32_t service_end_time;
} Customer;

#endif
