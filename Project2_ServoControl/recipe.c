#include "recipe.h"

/*
LoadnewRecipe will load a given recipe from a char pointer into 
	a RecipeThread ("thread") struct.
 */
void LoadNewRecipe(struct RecipeThread* thread, char recipie[], uint16_t size)
{
	thread->m_bRunningRecipe = 1;
	thread->m_bPaused = 0;
	
	thread->m_Index = 0;
	for (int i = 0; i < size; i++)
	{
		// Copy recipe
		thread->m_Recipie[i] = recipie[i];
	}
	
	thread->m_WaitIterations = 0;
	thread->m_WaitIterationsElapsed = 0;
	
	thread->m_LoopIndex = (-1);
	thread->m_LoopCount = 0;
	thread->m_LoopCountMax = 0;
	
	thread->m_CommandError = 0;
	thread->m_NestedLoopError = 0;
	
	thread->m_InSweep = 0;
}

void RestartRecipe(struct RecipeThread* thread)
{
		thread->m_bRunningRecipe = 1;
	thread->m_bPaused = 0;
	
	thread->m_Index = 0;
	
	thread->m_WaitIterations = 0;
	thread->m_WaitIterationsElapsed = 0;
	
	thread->m_LoopIndex = (-1);
	thread->m_LoopCount = 0;
	thread->m_LoopCountMax = 0;
	
	thread->m_CommandError = 0;
	thread->m_NestedLoopError = 0;
	
	thread->m_InSweep = 0;
}	


void TogglePauseRecipe(struct RecipeThread* thread, uint8_t pause)
{
	thread->m_bPaused = pause;
}


void RunRecipe(struct RecipeThread* thread)
{
	// Run the recipe until a RECIPE_END command is found.
		// If this command is never found, potential out-of-bounds with recipe array
	
	if (thread->m_bRunningRecipe && !thread->m_bPaused)
	{
		// Check if we're waiting. If not, then run the next recipe command.
		if (thread->m_WaitIterationsElapsed < thread->m_WaitIterations)
		{
			thread->m_WaitIterationsElapsed += 1;
		}
		// Check if we're sweeping.
		else if (thread->m_InSweep)
		{
			if (thread->m_InSweep == 1)
			{
				SetPWMPulsePosition(thread->servo, 5);
				
				thread->m_InSweep += 1;
			}
			else if (thread->m_InSweep == 2)
			{
				SetPWMPulsePosition(thread->servo, 0);
				
				thread->m_InSweep = 0;
			}
			
			thread->m_WaitIterations = WAIT_ITER_AFTER_MOV;
			thread->m_WaitIterationsElapsed = 0;
		}
		else
		{
			// Parse the command.
			uint8_t command = (uint8_t) thread->m_Recipie[thread->m_Index];
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
					thread->m_CommandError = 1;
					thread->m_bRunningRecipe = 0;
				}
				else
				{
					SetPWMPulsePosition(thread->servo, param);
					
					thread->m_WaitIterations = WAIT_ITER_AFTER_MOV;
					thread->m_WaitIterationsElapsed = 0;
				}
			}
			else if (opcode == OP_WAIT)
			{
				// Set into wait iterations the amount to wait, and clear the elapsed wait counter.
				thread->m_WaitIterations = param;
				thread->m_WaitIterationsElapsed = 0;
			}
			else if (opcode == OP_LOOP)
			{
				if (thread->m_LoopIndex >= 0)
				{
					// We already started looping. This is a nested loop. 
					// Set an error and stop execution.
					thread->m_NestedLoopError = 1;
					
					thread->m_bRunningRecipe = 0;
				}
				else
				{
					thread->m_LoopIndex = thread->m_Index;
					thread->m_LoopCount = 0;
					thread->m_LoopCountMax = param;
				}
			}
			else if (opcode == OP_ENDLOOP)
			{
				if (thread->m_LoopCount < thread->m_LoopCountMax)
				{
					thread->m_Index = thread->m_LoopIndex;
						// LoopIndex is the index of the LOOP command. 
						// We will iterate and go to the first command in the LOOP after.
				}
				else
				{
					// We've done enough loops. Clear loop variables.
					thread->m_LoopIndex = (-1);
					thread->m_LoopCount = 0;
					thread->m_LoopCountMax = 0;
				}
			}
			else if (opcode == OP_SWEEP)
			{
				thread->m_InSweep = 1;
				SetPWMPulsePosition(thread->servo, 0);
				
				thread->m_WaitIterations = WAIT_ITER_AFTER_MOV;
				thread->m_WaitIterationsElapsed = 0;
			}
			else if (opcode == OP_RECIPEEND)
			{
				// End program execution.
				thread->m_bRunningRecipe = 0;
			}
			else if (opcode == OP_PAUSE)
			{
				// Pause program execution
				thread->m_bPaused = 1;
			}
			else
			{
				// Set an error and stop execution.
				thread->m_CommandError = 1;
				thread->m_bRunningRecipe = 0;
			}
			
			// Loop to next command.
			thread->m_Index += 1;
		}
	}
}
