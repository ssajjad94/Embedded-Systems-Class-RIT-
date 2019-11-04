#include "recipe_task.h"

/*
	Task execution. 
*/
void recipe_task(void *parameters)
{
	RECIPE_PARAMS_t *p = (RECIPE_PARAMS_t *)parameters;
	uint8_t id = p->id;
	
	while (1)
	{
		// Do thread stuff here
		RunRecipe(id);
	}
}


/*
	Task initizialization.
*/
void recipe_task_init(int id, char *recipe_task_name, char recipie[], uint16_t size)
{
	// Get pointer to THIS instance of parameters (one for each task)
	RECIPE_PARAMS_t *p = &recipe_params[id];   
	
	// Initialize task parameters
	p->id = id; // 0 or 1
	strncpy(p->task_name, recipe_task_name, configMAX_TASK_NAME_LEN);
	
	// Initialize additonal task variables (adapted from Project 2)
	p->servo = id; // 0 or 1
	
	p->m_bRunningRecipe = 1;
	p->m_bPaused = 0;
	p->m_Index = 0;
	
	p->m_WaitIterations = 0;
	p->m_WaitIterationsElapsed = 0;
	
	p->m_LoopIndex = (-1);
	p->m_LoopCount = 0;
	p->m_LoopCountMax = 0;
	
	p->m_CommandError = 0;
	p->m_NestedLoopError = 0;
	
	p->m_InSweep = 0;
	
	// Copy the recipe
	for (int i = 0; i < size; i++)
	{
		p->m_Recipie[i] = recipie[i];
	}
	
	// Create the task
  xTaskCreate( recipe_task, "TELLER TASK", 256, (void *)p, 2, &p->han