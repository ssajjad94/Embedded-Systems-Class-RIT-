#include "UserCommand.h"

char inputBuffer[3];

/*
	// Returns non-zero if a full input is ready,
	//		otherwise, returns zero until it's ready.
*/
uint8_t CheckForInputs(void)
{
	// Try to read a char
	char rxByte[1];
	rxByte[0] = USART_ReadAsync(USART2);
	if (rxByte[0] != 0x00)
	{
		// Just in case -> convert \n\r to \n. We only want one.
		if (rxByte[0] == '\n' || rxByte[0] == '\r')
			rxByte[0] = '\n';
		
		// Check if buffer is already full
			// If it's already full, stop taking inputs until ClearInputBuffer() is called.
		if (inputBuffer[2] != '\n')
		{
			// Shift input buffer down
			inputBuffer[0] = inputBuffer[1];
			inputBuffer[1] = inputBuffer[2];
			inputBuffer[2] = rxByte[0];
		}
	}
	
	// If inputBuffer[2] == <CR>, then an input is ready
	if (inputBuffer[2] == '\n')
		return 1;
	else
		return 0;
}

char GetServoOneChar(void)
{
	return inputBuffer[0];
}

char GetServoTwoChar(void)
{
	return inputBuffer[1];
}

void ClearInputBuffer(void)
{
	// Clear the Servo 1 command char
	inputBuffer[0] = 0x00;
	
	// Clear the Servo 2 command char
	inputBuffer[1] = 0x00;
	
	// Clear the <CR> command char
	inputBuffer[2] = 0x00;
}

void PerformCommand(struct RecipeThread* thread, char command)
{
	uint8_t error = thread->m_CommandError || thread->m_NestedLoopError;
	uint8_t running = thread->m_bRunningRecipe;
	uint8_t paused = thread->m_bPaused;
	
	// Perform user commands
	switch (command)
	{
		case 'P':
		case 'p':
			// Pause recipe execution. Not operative after recipe end or error.
			if (running && !error)
			{
				TogglePauseRecipe(thread, 1);
			}
			break;
		
		case 'C':
		case 'c':
			// Continue recipe execution. Not operative after end or error.
			if (running && !error)
			{
				TogglePauseRecipe(thread, 0);
			}
			break;
		
		case 'R':
		case 'r':
			if (!paused)
			{
				if (thread->servo == 1)
				{
					SetPWMPulsePosition1(PWMOneLastPosition + 1);
				}
				else if (thread->servo == 2)
				{
					SetPWMPulsePosition2(PWMTwoLastPosition + 1);
				}
			}
			break;
		
		case 'L':
		case 'l':
			if (!paused)
			{
				if (thread->servo == 1)
				{
					SetPWMPulsePosition1(PWMOneLastPosition - 1);
				}
				else if (thread->servo == 2)
				{
					SetPWMPulsePosition2(PWMTwoLastPosition - 1);
				}
			}
			break;
		
		case 'B':
		case 'b':
			RestartRecipe(thread);
			break;
		
		case 'N':
		case 'n':
		default:
			break;
	}
}
