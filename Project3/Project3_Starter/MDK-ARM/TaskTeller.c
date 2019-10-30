#include "TaskTeller.h"

TELLERSERVICE_PARAMS_t teller_service_params[NUMBER_OF_TELLERS];

/*
	Task execution. 
*/
void teller_service_task(void *parameters)
{
	TELLERSERVICE_PARAMS_t *p = (TELLERSERVICE_PARAMS_t *) parameters; 
	Teller* teller = p->teller;
	int tellerID = p->tellerID;
	
	while (1)
	{
		if (teller->status == Idle)
		{
			// If the teller is idle, they will try to take a break if they can or
			//		take a customer from the queue.
			if (CanGoOnBreak(tellerID))
			{
				StartBreak(tellerID);
			}
			else
			{
				// Try to take a customer from the queue. 
				TakeCustomerFromQueue(tellerID);
			}
		}
		else if (teller->status == Busy)
		{
			// If the teller is busy, they are currently servicing a customer.
			//		Continue servicing the customer until they are done.
			if (IsCurrentCustomerServiced(tellerID))
			{
				EndCurrentCustomer(tellerID);
			}
		}
		else if (teller->status == OnBreak)
		{
			// If the teller is on break, they are on break until the break is over.
			if (IsCurrentBreakOver(tellerID))
			{
				EndBreak(tellerID);
			}
		}
	}
}


/*
	Task initizialization.
*/
void teller_service_task_init(char *teller_service_task_name, int id,
																uint32_t* simulation_clock_ptr,
																Teller* my_teller,
																QueueHandle_t* customer_queue_ptr
)
{
	// Get pointer to THIS instance of parameters (one for each task)
	TELLERSERVICE_PARAMS_t *p = &teller_service_params[id];   
	p->tellerID = id;
	
	// Initialize members of this structure for this task
  strncpy(p->task_name, teller_service_task_name, configMAX_TASK_NAME_LEN);
	p->SimulationClockPtr = simulation_clock_ptr;
	p->teller = my_teller;
	p->CustomerQueuePtr = customer_queue_ptr;
	
	
	// Create the task
  xTaskCreate( teller_service_task, "TELLER TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Try to remove a customer from the customer queue ptr.
	If successfully (if there was a customer in the queue),
		add the customer to the teller's list of serviced custemers,
		and set that customer service start time to now (using the simulation clock ptr)
		and set the service end time to random value 30s to 8 minutes later.
		Set Teller's status to Busy.
	If not customer, do nothing.
*/																
void TakeCustomerFromQueue(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// Get a customer from the queue pointer
	QueueHandle_t queue = *((&teller_service_params[tellerID])->CustomerQueuePtr);
	
	Customer nextCustomer;
	if (pdPASS == xQueueReceive(queue, &nextCustomer, 0))
	{
		// Data was successful
		
		// Set the customer's start time to now
		nextCustomer.service_start_time = (*simulation_clock_ptr);
		
		// Calculate a random time from 30 seconds to 8 minutes
		uint32_t random_time;
		uint32_t min_time = MIN_CUSTOMER_SERVICE_TIME;
		uint32_t max_time = MAX_CUSTOMER_SERVICE_TIME;
		HAL_RNG_GenerateRandomNumber(&hrng, &random_time);
		random_time = min_time + (random_time % (max_time - min_time));
		
		// Set the customer's end time to random_time later
		nextCustomer.service_end_time = (*simulation_clock_ptr) + random_time;
		
		// Add that customer to this teller's list of customers
		teller->serviced_customers[teller->serviced_customers_cnt] = nextCustomer;
		teller->serviced_customers_cnt++;
		
		// The teller is now Busy
		teller->status = Busy;
	}
	else
	{
		// Not successful -> the queue was empty.
		// The teller should stay idle.
		teller->status = Idle;
	}
}


/*
	If the teller is busy, checks the current customer that the 
		teller is servicing. If the customer's service end time has passed,
		finish servicing that customer.

 */																
uint8_t IsCurrentCustomerServiced(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// Get the current customer
	Customer currCustomer = teller->serviced_customers[teller->serviced_customers_cnt - 1];

	// Check if the customer's end time has passed
	if (*(simulation_clock_ptr) > currCustomer.service_end_time)
	{
		return 1;
	}
	
	return 0;
}															


/*
		Set the teller back to Idle.
*/
void EndCurrentCustomer(int tellerID)
{
	Teller* teller = (&teller_service_params[tellerID])->teller;
	
	teller->status = Idle;
}
			
/*
	Checks if the teller is able to go on break. 
  Teller can go on break if break start time has elapsed.
		Returns 1 if so, 0 if not
*/																
uint8_t CanGoOnBreak(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// If there is a break scheduled
	if (teller->break_cnt > 0)
	{
		// Get the newest break
		Break newestBreak = teller->breaks[teller->break_cnt - 1];
		
		// Check that this is a new break that hasn't previously finished
		if (newestBreak.end_time == 0) 
		{
			// Check that this break's start time has elapsed.
			if (*(simulation_clock_ptr) > newestBreak.start_time)
			{
				return 1;
			}
		}
	}
	
	return 0;
}


/*
	Set the Teller's status to OnBreak.
	Set the current break's start time to now
	Set the current break's end time to random time 1-4 minutes later.
*/
void StartBreak(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// Set the Teller's status
	teller->status = OnBreak;
	
	// Get the next break
	Break currBreak = teller->breaks[teller->break_cnt - 1];
	
	// Set the break's start time to now 
		// (in cases where the break start later than intended)
	currBreak.start_time = *(simulation_clock_ptr);
	
	// Calculate a random time from 1 to 4 minutes
	uint32_t random_time;
	uint32_t min_time = MIN_BREAK_TIME;
	uint32_t max_time = MAX_BREAK_TIME;
	HAL_RNG_GenerateRandomNumber(&hrng, &random_time);
	random_time = min_time + (random_time % (max_time - min_time)); 
	
	// Set the break's end time to random time 1-4 minutes later
	currBreak.end_time = *(simulation_clock_ptr) + random_time;
	
	//Write back
	teller->breaks[teller->break_cnt - 1] = currBreak;
}


/*
	Checks if current break's end time has passed. If so, return 1.
		If not, return 0.
  After
*/
uint8_t IsCurrentBreakOver(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// Get the newest break (the break that we're on)
	Break currBreak = teller->breaks[teller->break_cnt - 1];
	
	// Check if the break's end_time has elapsed (the break is over)
	if (*(simulation_clock_ptr) > currBreak.end_time)
	{
		return 1;
	}
	
	return 0;
}


/*
	Set the Teller's status back to Idle.
	Creates (schedules) the next break, and 
	  sets its start time to 30-60 minutes later.
	Add that break to the list of breaks.
 */
void EndBreak(int tellerID)
{
	// Get the teller and time
	Teller* teller = (&teller_service_params[tellerID])->teller;
	uint32_t* simulation_clock_ptr = (&teller_service_params[tellerID])->SimulationClockPtr;
	
	// Previous break
	Break prevBreak = teller->breaks[teller->break_cnt - 1];
	
	// The teller's break is over -> set it back to idle
	teller->status = Idle;
	
	// Create and schedule the next break
	Break newBreak;
	
	// Calculate a random time from 30 to 60 minutes
	uint32_t random_time;
	uint32_t min_time = MIN_NEXT_BREAK_TIME;
	uint32_t max_time = MAX_NEXT_BREAK_TIME;
	HAL_RNG_GenerateRandomNumber(&hrng, &random_time);
	random_time = min_time + (random_time % (max_time - min_time));
	
	// Set the break's start time to that random time later after the previous break's start
	newBreak.start_time = prevBreak.start_time + random_time;
	newBreak.end_time = 0; // Default 


	// Only add the break if it's before the bank closes
	if (newBreak.start_time < BANK_CLOSE_TIME)
	{
		// Add this break to the teller's list of breaks
		teller->breaks[teller->break_cnt] = newBreak;
		
		// Increment how many breaks this teller has been on
		teller->break_cnt++;
	}
}

