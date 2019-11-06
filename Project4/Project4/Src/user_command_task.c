#include "user_command_task.h"

/*
	Task execution. 
*/
void user_command_task(void *parameters)
{
	while (1)
	{
		CheckToClearBuffer();
		
		PollForInputs();
	}
}


/*
	Task initizialization.
*/
void user_command_task_init(char *user_command_task_name)
{
	// Get pointer to THIS instance of parameters
	USER_COMMAND_PARAMS_t *p = &user_command_params;   
	
	// Initialize task parameters
	strncpy(p->task_name, user_command_task_name, configMAX_TASK_NAME_LEN);
	
	// Create the task
  xTaskCreate(user_command_task, "USER COMMAND TASK", 256, (void *)p, 2, &p->handle);
}


/*
	This function will check if the buffer has been set as full
		with an \n in the last slot. 
	If so, it will check if the slots for the two servos have
		already been popped (set to 0). If so, clear the third
		slot so PollForInputs can continue reading.
*/
void CheckToClearBuffer(void)
{
	// Get pointer to THIS instance of parameters 
	USER_COMMAND_PARAMS_t *p = &user_command_params;
	char* inputBuffer = p->inputBuffer;
	
	// Check if we should clear the buffer
	if (inputBuffer[2] == '\n')
	{
		// If both values have been read, then clear it
		if (inputBuffer[0] == 0 && inputBuffer[1] == 0)
		{ 
			inputBuffer[2] = 0;
		}
	}
}


/*
	Try to read a character from the UART and push to a buffer.
*/
void PollForInputs(void)
{
	// Get pointer to THIS instance of parameters
	USER_COMMAND_PARAMS_t *p = &user_command_params;
	char* inputBuffer = p->inputBuffer;
	
	// Try to read a char
	char rxByte[1];
	// rxByte[0] = USART_ReadAsync(USART2);
	rxByte[0] = USART_ReadAsync();
	
	
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
}


/*
	Removes a character from the buffer corresponding to a servo.
*/
char PopServoChar(uint8_t servo)
{
	// Get pointer to THIS instance of parameters
	USER_COMMAND_PARAMS_t *p = &user_command_params;
	char* inputBuffer = p->inputBuffer;
	
	if (inputBuffer[2] == '\n')
	{
		// Get the char to return
		char retVal = inputBuffer[servo];

		// Clear / pop it
		inputBuffer[servo] = 0;

		// Return original
		return retVal;
	}
	else
	{
		return 0;
	}
}


