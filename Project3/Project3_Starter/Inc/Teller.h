#ifndef __TELLER_H    // header files should include guards
#define __TELLER_H

#include "stm32l4xx_hal.h"
#include "Customer.h"
#include "Break.h"

#define NUMBER_OF_TELLERS		(3)
#define LIST_OF_CUSTOMERS_LENGTH	(1) //(421)
#define LIST_OF_BREAKS_LENGTH		  (1) // (15)

enum Status 
{
	Idle = 0,
	Busy = 1,
	OnBreak = 2
};


typedef struct 
{
	enum Status status;
	
	Customer *serviced_customers;
	uint16_t serviced_customers_cnt;
	
	Break *breaks;
	uint16_t break_cnt;
} Teller;

#endif
