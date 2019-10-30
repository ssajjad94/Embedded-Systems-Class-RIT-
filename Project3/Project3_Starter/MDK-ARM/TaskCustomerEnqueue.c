#include "TaskCustomerEnqueue.h"

CUSTOMERENQUEUE_PARAMS_t customer_enqueue_params;

/*
	Task execution. 
*/
void customer_enqueue_task(void *parameters)
{
	while(1)
	{
		if (CanNextCustomerBeQueued())
		{
			AddCustomer();
		}
	}
}


/*
	Task initizialization.
*/
void customer_enqueue_task_init(char *customer_enqueue_task_name,
																uint32_t* simulation_clock_ptr,
																Customer* customer_ptr,
																uint32_t* customer_ptr_size,
																QueueHandle_t* customer_queue_ptr,
																uint32_t* maximum_customer_queue_cnt_ptr,
																uint32_t* next_customer_time_ptr
)
{
	// Get pointer to THIS instance of parameters (one for each task)
	CUSTOMERENQUEUE_PARAMS_t *p = &customer_enqueue_params;   
	
	// Initialize members of this structure for this task
  strncpy(p->task_name, customer_enqueue_task_name, configMAX_TASK_NAME_LEN);
	p->SimulationClockPtr = simulation_clock_ptr;
	p->CustomerPtr = customer_ptr;
	p->CustomerPtrSize = customer_ptr_size;
	p->CustomerQueuePtr = customer_queue_ptr;
	p->MaximumCustomerQueueCntPtr = maximum_customer_queue_cnt_ptr;
	p->NextCustomerTimePtr = next_customer_time_ptr;
	
	// Create the task
  xTaskCreate( customer_enqueue_task, "UPDATE TASK", 256, (void *)p, 2, &p->handle);
}


/*
	Checks if the current simulated time is between bank operating hours (9 to 4)
		and if so, checks if the Next Customer Time has elapsed. If so, return true (1);
		if not, return 0.
*/
uint8_t CanNextCustomerBeQueued()
{
	uint8_t retVal = 0;
	CUSTOMERENQUEUE_PARAMS_t *p = &customer_enqueue_params;
	
	uint8_t isBankOpen = ( *(p->SimulationClockPtr) >= BANK_START_TIME ) 
											&& ( *(p->SimulationClockPtr) <= BANK_CLOSE_TIME);
	if (isBankOpen)
	{
		if (*(p->NextCustomerTimePtr) >= *(p->SimulationClockPtr))
		{
			retVal = 1;
		}
	}
	
	return retVal;
}


/*
	Creates a new customer. Set it's start time to current simulated time.
	Adds this new customer to the customer queue 
		Updates the maximum customer queue count if larger. 
	Adds this new customer to the list of all customers.
		Updates the size of the list of all 
*/
void AddCustomer()
{
	CUSTOMERENQUEUE_PARAMS_t *p = &customer_enqueue_params;
	
	// Get the queue
	QueueHandle_t queue = *(p->CustomerQueuePtr);
	
	// Create the new customer
	Customer new_customer;
	
	// Set it's start to now
	new_customer.arrival_time = *(p->SimulationClockPtr);
	
	// Add this new customer to the customer queue
	if (pdPASS == xQueueSend(queue, &new_customer, 0))
	{
		// Count how many customers are in the queue
		uint32_t customerQueueCnt = uxQueueMessagesWaiting(queue);
		
		// If the queue count is larger than the largest queue count, update the largest queue count
		if (customerQueueCnt > *(p->MaximumCustomerQueueCntPtr))
		{
			*(p->MaximumCustomerQueueCntPtr) = customerQueueCnt;
		}
		
		// Add this new customer to the list of all customers
		p->CustomerPtr[*(p->CustomerPtrSize)] = new_customer;
		
		// Update the size of the list
		*(p->CustomerPtrSize) =  *(p->CustomerPtrSize) + 1;
	}
}
