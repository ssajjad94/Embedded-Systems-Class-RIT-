#include "TaskUpdate.h"

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
	//@todo
}

/*
	This function will update the current display.
		Display will print current simulated time, the number
		of customers waiting (size of customerQueuePtr), and the
		status of each teller. 
*/
void UpdateDisplay()
{
	//@todo
}
