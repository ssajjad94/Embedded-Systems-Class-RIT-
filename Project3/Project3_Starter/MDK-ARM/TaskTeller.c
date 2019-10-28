#include "TaskTeller.h"

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
	Teller* teller = (&teller_service_params[tellerID])->teller;
	//@todo
}


/*
	If the teller is busy, checks the current customer that the 
		teller is servicing. If the customer's service end time has passed,
		finish servicing that customer.

 */																
uint8_t IsCurrentCustomerServiced(int tellerID)
{
	Teller* teller = (&teller_service_params[tellerID])->teller;
	//@todo
}															


/*
		If the teller can go on break, go to OnBreak.
		If not, go to Idle. 
*/
void EndCurrentCustomer(int tellerID)
{
	Teller* teller = (&teller_service_params[tellerID])->teller;
	//@todo
}
			
/*
	Checks if the teller is able to go on break. 
  Teller can go on break if break start time has elapsed.
		Returns 1 if so, 0 if not
*/																
uint8_t CanGoOnBreak(int tellerID)
{
	Teller* teller = (&teller_service_params[tellerID])->teller;
	//@todo
}


/*
	Set the Teller's status to OnBreak.
	Set the current break's end time to random time 1-4 minutes later.
	Add that break to the list of breaks.
*/
void StartBreak(int tellerID)
{
	Teller* teller = (&teller_service_params[tellerID])->teller;
	//@todo
}


/*
	Checks if current break's end time has passed. If so, return 1.
		If not, return 0.
*/
uint8_t IsCurrentBreakOver(int tellerID)
{
	//@todo
}


/*
	Set the Teller's status back to Idle.
	Creates (schedules) the next break, and 
	  sets its start time to 30-60 minutes later.
 */
void EndBreak(int tellerID)
{
	//@todo
}

