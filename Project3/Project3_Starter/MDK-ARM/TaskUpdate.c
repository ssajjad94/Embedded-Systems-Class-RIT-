#include "TaskUpdate.h"

UPDATE_PARAMS_t update_params;

/*
	Task execution. 
*/
void update_task(void *parameters)
{
	USART_Printf("\033[2J");
	
	while(1)
	{
		// USART_Printf("\033[2J");	// Clear screen
		USART_Printf("\033[0;0H");	// 0,0
		
		// Update current time in main program
		UpdateTime();
		
		// Update display with data
		UpdateDisplay();
		
		// Update metrics
		DisplayMetrics();
	}
}


/*
	Task initizialization.
*/
void update_task_init(char *update_task_name, uint32_t* simulation_clock_ptr, 
												QueueHandle_t* customer_queue_ptr, Teller* teller_ptr)
{
	// Get pointer to THIS instance of parameters (one for each task)
	UPDATE_PARAMS_t *p = &update_params;   
	
	// Initialize members of this structure for this task
  strncpy(p->task_name, update_task_name, configMAX_TASK_NAME_LEN);
	p->SimulationClockPtr = simulation_clock_ptr;
	p->CustomerQueuePtr = customer_queue_ptr;
	p->TellerPtr = teller_ptr;
	
	// Create the task
  xTaskCreate( update_task, "UPDATE TASK", 256, (void *)p, 2, &p->handle);
}

/*
	This function will update the current time in the main program,
		which can be accesed with SimulationClockPtr.
*/
void UpdateTime()
{
	// Calculate the real elapsed time
	uint32_t tickCount = xTaskGetTickCount();
	uint32_t timeInMilliseconds = tickCount / ((configTICK_RATE_HZ / 1000)) ;
	
	// Convert the real time to simulation time
		// 100ms in real time = 1 minute in simulation time
		// 	so 1 second in real time = 600 seconds in simulation time
	uint32_t simulationTimeInMilliseconds = timeInMilliseconds * 60;
	uint32_t simulationTimeInSeconds = simulationTimeInMilliseconds / 1000;
	
	// Set the simulation clock to the elapsed time
	UPDATE_PARAMS_t *p = &update_params;
	*(p->SimulationClockPtr) = simulationTimeInSeconds;
}

/*
	This function will update the current display.
		Display will print current simulated time, the number
		of customers waiting (size of customerQueuePtr), and the
		status of each teller. 
*/
void UpdateDisplay()
{
	// Get the data to print
	UPDATE_PARAMS_t *p = &update_params;
	
	// Get the queue
	QueueHandle_t queue = *(p->CustomerQueuePtr);
	
	// Print current time
	uint32_t currentTime = *(p->SimulationClockPtr);
	
	uint16_t currentTimeHours = 7 + (currentTime / T_HOUR);
	if (currentTimeHours > 12)
		currentTimeHours -= 12;
	uint16_t currentTimeMins = (currentTime % T_HOUR) / T_MINUTE;
	
	USART_Printf("Current time (in seconds): %5d \t %2.d:%2.d\n\r", currentTime, 
				currentTimeHours, currentTimeMins);
	
	// Print number of customers waiting in the queue
	uint16_t numberOfCustomers = uxQueueMessagesWaiting(queue);
	USART_Printf("Number of customers on the queue: %d \n\r", numberOfCustomers);
	
	// Print status of each teller
	for (int tellerID = 0; tellerID < 3; tellerID++)
	{
		Teller current_teller = p->TellerPtr[tellerID];
		
		USART_Printf("Teller #%d status: ", tellerID);
		
		if (current_teller.status == Idle)
		{
			USART_Printf("IDLE     \n\r");
		}
		else if (current_teller.status == Busy)
		{
			USART_Printf("BUSY     \n\r");
		} 
		else if (current_teller.status == OnBreak)
		{
			USART_Printf("ON_BREAK\n\r");
		}
		else
		{
			// Error case. Should not enter here.
			USART_Printf("UNKNOWN_ERR\n\r");
		}
		
		// Print the total customers serviced
		uint16_t cust_cnt = current_teller.serviced_customers_cnt;
		if (current_teller.status == Busy)
		{
			// Don't count the current customer
			cust_cnt -= 1;
		}
		USART_Printf("\tServiced %d customers\n\r", cust_cnt);
		
		// Print the number of breaks taken
		uint16_t break_cnt = current_teller.break_cnt;
		uint16_t nextBreakTime = BANK_CLOSE_TIME - *(p->SimulationClockPtr); 
		uint16_t timeLeftOnBreak = 0;
		
		// Status flags
		uint8_t on_break = 0;
		uint8_t waiting_for_customer = 0;
		
		// If we have breaks
		if (current_teller.break_cnt > 0)
		{
			Break nextBreak = current_teller.breaks[ current_teller.break_cnt - 1];
			if (nextBreak.end_time == 0)
			{
				// If we haven't started the last break yet
				break_cnt -= 1;
				
				if (*(p->SimulationClockPtr) >= nextBreak.start_time)
				{
					nextBreakTime = 0;	// Waiting for customer while busy
					waiting_for_customer = 1;
				}
				else
				{
					nextBreakTime = nextBreak.start_time - *(p->SimulationClockPtr);
				}
			}
			else
			{
				nextBreakTime = 0;	// Already on break
				timeLeftOnBreak = nextBreak.end_time - *(p->SimulationClockPtr);
				on_break = 1;
			}
		}
		
		// Convert to minutes
		timeLeftOnBreak = timeLeftOnBreak / T_MINUTE;
		nextBreakTime = nextBreakTime / T_MINUTE;
		
		// Print depending on status
		USART_Printf("\tTaken %d breaks. \n\r", current_teller.break_cnt - 1);
		if (on_break)
		{
			USART_Printf("\tCurrently enjoying a break ");
			USART_Printf("for %d more minutes.     \n\r", timeLeftOnBreak);
		}
		else
		{
			if (waiting_for_customer)
			{
				USART_Printf("\tLooking forward to a break ");
				USART_Printf("after this customer.    \n\r");
			}
			else
			{
				USART_Printf("\tLooking forward to a break ");
				USART_Printf("in %d minutes.           \n\r", nextBreakTime);
			}
		}
	}
}
