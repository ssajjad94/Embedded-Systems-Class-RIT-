#include "TaskUpdate.h"

UPDATE_PARAMS_t update_params;

/*
	Task execution. 
*/
void update_task(void *parameters)
{
	while(1)
	{
		// Update current time in main program
		UpdateTime();
		
		// Update display with data
		UpdateDisplay();
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
	uint32_t timeInMilliseconds = (tickCount / (configTICK_RATE_HZ / 1000)) ;
	
	// Convert the real time to simulation time
		// 100ms in real time = 1 second in simulation time
		// 	so 1 second in real time = 10 seconds in simulation time
	uint32_t simulationTimeInMilliseconds = timeInMilliseconds * 10;
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
	USART_Printf("Current time (in seconds): %d \n\r", currentTime);
	
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
			USART_Printf("IDLE\n\r");
		}
		else if (current_teller.status == Busy)
		{
			USART_Printf("BUSY\n\r");
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
	}
}
