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
		// Check if there is a command
		char command = PopServoChar(id);
		if (command != 0)
		{
			// Run that command 
			PerformCommand(id, command);
		}
		
		// Do thread stuff here
		RunRecipe(id);
		
		// Delay
		WaitTIM3(1000);
		
		if (id == 0)
		{
			// Display status if it's the first (0th) servo
			PollForLEDErrors(id);
		}
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
  xTaskCreate( recipe_task, "RECIPE TASK", 256, (void *)p, 2, &p->handle);
}


/*
	Restart Recipe
*/
void RestartRecipe(uint8_t id)
{
	// Get pointer to THIS instance of parameters (one for each task)
	RECIPE_PARAMS_t *p = &recipe_params[id];   
	
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
}


/* 
	Toggles whether or not the recipe is paused.
*/
void TogglePauseRecipe(uint8_t id, uint8_t pause)
{
	// Get pointer to THIS instance of parameters (one for each task)
	RECIPE_PARAMS_t *p = &recipe_params[id];  
	
	p->m_bPaused = pause;
}


/*
	Tries to load and run next recipe execution
*/
void RunRecipe(uint8_t id)
{
	// Get pointer to THIS instance of parameters (one for each task)
	RECIPE_PARAMS_t *p = &recipe_params[id];   

	// Run the recipe until a RECIPE_END command is found.
	// If this command is never found, potential out-of-bounds with recipe array
	
	if (p->m_bRunningRecipe && !p->m_bPaused)
	{
		// Check if we're waiting. If not, then run the next recipe command.
		if (p->m_WaitIterationsElapsed < p->m_WaitIterations)
		{
			p->m_WaitIterationsElapsed += 1;
		}
		// Check if we're sweeping.
		else if (p->m_InSweep)
		{
			if (p->m_InSweep == 1)
			{
				SetPWMPulsePosition(p->servo, 5);
				
				p->m_InSweep += 1;
			}
			else if (p->m_InSweep == 2)
			{
				SetPWMPulsePosition(p->servo, 0);
				
				p->m_InSweep = 0;
			}
			
			p->m_WaitIterations = WAIT_ITER_AFTER_MOV;
			p->m_WaitIterationsElapsed = 0;
		}
		else
		{
			// Parse the command.
			uint8_t command = (uint8_t) p->m_Recipie[p->m_Index];
			// command
				// first three bits is the Opcode
				// last five bits is the parameter
			
			uint8_t opcode = (command & OPCODE_MASK) >> OPCODE_SHIFT;
			uint8_t param = (command & PARAM_MASK);
			
			if (opcode == OP_MOV)
			{
				if (param > 5) // If param is invalid
				{
					// Set an error and stop execution.
					p->m_CommandError = 1;
					p->m_bRunningRecipe = 0;
				}
				else
				{
					// If param is 0 to 5 
					SetPWMPulsePosition(p->servo, param);
					
					p->m_WaitIterations = WAIT_ITER_AFTER_MOV;
					p->m_WaitIterationsElapsed = 0;
				}
			}
			else if (opcode == OP_WAIT)
			{
				// Set into wait iterations the amount to wait, and clear the elapsed wait counter.
				p->m_WaitIterations = param;
				p->m_WaitIterationsElapsed = 0;
			}
			else if (opcode == OP_LOOP)
			{
				if (p->m_LoopIndex >= 0)
				{
					// We already started looping. This is a nested loop. 
					// Set an error and stop execution.
					p->m_NestedLoopError = 1;
					p->m_bRunningRecipe = 0;
				}
				else
				{
					p->m_LoopIndex = p->m_Index;
					p->m_LoopCount = 0;
					p->m_LoopCountMax = param;
				}
			}
			else if (opcode == OP_ENDLOOP)
			{
				if (p->m_LoopCount < p->m_LoopCountMax)
				{
					p->m_Index = p->m_LoopIndex;
						// LoopIndex is the index of the LOOP command. 
						// We will iterate and go to the first command in the LOOP after.
				}
				else
				{
					// We've done enough loops. Clear loop variables.
					p->m_LoopIndex = (-1);
					p->m_LoopCount = 0;
					p->m_LoopCountMax = 0;
				}
			}
			else if (opcode == OP_SWEEP)
			{
				p->m_InSweep = 1;
				SetPWMPulsePosition(p->servo, 0);
				
				p->m_WaitIterations = WAIT_ITER_AFTER_MOV;
				p->m_WaitIterationsElapsed = 0;
			}
			else if (opcode == OP_RECIPEEND)
			{
				// End program execution.
				p->m_bRunningRecipe = 0;
			}
			else if (opcode == OP_PAUSE)
			{
				// Pause program execution
				p->m_bPaused = 1;
			}
			else
			{
				// Set an error and stop execution.
				p->m_CommandError = 1;
				p->m_bRunningRecipe = 0;
			}
			
			// Loop to next command.
			p->m_Index += 1;
		}
	}
}


void PerformCommand(uint8_t id, char command)
{
	// Get pointer to THIS instance of parameters (one for each task)
	RECIPE_PARAMS_t *p = &recipe_params[id]; 
	
	uint8_t error = p->m_CommandError || p->m_NestedLoopError;
	uint8_t running = p->m_bRunningRecipe;
	uint8_t paused = p->m_bPaused;
	
	// Perform user commands
	switch (command)
	{
		case 'P':
		case 'p':
			// Pause recipe execution. Not operative after recipe end or error.
			if (running && !error)
			{
				TogglePauseRecipe(id, 1);
			}
			break;
		
		case 'C':
		case 'c':
			// Continue recipe execution. Not operative after end or error.
			if (running && !error)
			{
				TogglePauseRecipe(id, 0);
			}
			break;
		
		case 'R':
		case 'r':
			// If paused, move servo right.
			if (paused)
			{
				ShiftPWMRight(p->servo);
			}
			break;
		
		case 'L':
		case 'l':
			// If paused, move servo left.
			if (paused)
			{
				ShiftPWMLeft(p->servo);
			}
			break;
		
		case 'B':
		case 'b':
			RestartRecipe(id);
			break;
		
		case 'N':
		case 'n':
		default:
			break;
	}
}

void PollForLEDErrors(uint8_t id)
{
	// Get pointer to THIS instance of parameters
	RECIPE_PARAMS_t *p = &recipe_params[id]; 
	
	// Check for errors in Servo One
		if (p->m_NestedLoopError)
		{
			Green_LED_On();
			Red_LED_On();
		}
		else if (p->m_CommandError)
		{
			Green_LED_Off();
			Red_LED_On();
		}
		else if (p->m_bPaused)
		{
			Green_LED_Off();
			Red_LED_Off();
		}
		else if (p->m_bRunningRecipe )
		{
			// If not paused or in any error state and is running:
			Green_LED_On();
			Red_LED_Off();
		}
}
