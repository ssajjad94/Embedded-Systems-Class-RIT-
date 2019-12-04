#include "display_task.h"

DISPLAY_TASK_PARAMS_t display_task_params;

/*
	Task initizialization.
*/
void display_task_init(char *display_task_name)
{
	// Get pointer to THIS instance of parameters
	DISPLAY_TASK_PARAMS_t *p = &display_task_params;   
	
	// Initialize task parameters
	strncpy(p->task_name, display_task_name, configMAX_TASK_NAME_LEN);
	
	// Create the task (at a lower priority)
  xTaskCreate(display_task, "DISPLAY TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Task execution. 
*/
void display_task(void *parameters)
{
	USART_Printf("\033[2J");
	
	float score = 0;
	uint32_t elapsedTime = 0;
	
	uint8_t playerPos = 0;
	uint8_t computerPos = 0;
	
	
	while (1)
	{
		// Update the display if game is still active
		//		(includes: score, time, player position, computer position
		if ((&game_params)->gameoverState == 0)
		{
			score = (&game_params)->score;
			elapsedTime = (&game_params)->elapsedTime;
			
			playerPos = (&servo_user_params)->servo_position;
			computerPos = (&servo_computer_params)->servo_position;
		}
		else
		{
			continue;
		}
		
		USART_Printf("\033[0;0H");	// 0,0
		USART_Printf("Level: %3d\n\r", (&game_params)->levelNum);
		USART_Printf("Score: %f\n\r", score);
		USART_Printf("Time: %f seconds\n\r", ((float) elapsedTime) / ( (float) TICKS_PER_SECOND) );
		// USART_Printf("Time: %u ticks\n\r", elapsedTime);
		// USART_Printf("Time: %u start ticks\n\r", (&game_params)->levelStartTime);
		// USART_Printf("Time: %u curr  ticks\n\r", TIM3->CNT);
		USART_Printf("\n\r");
		USART_Printf("Player   Pos: %4d\n\r", playerPos);
		USART_Printf("Computer Pos: %4d\n\r", computerPos);
		USART_Printf("\n\r");
		
		
		if ((&game_params)->gameoverState)
		{
			USART_Printf("GAME OVER \n\r");
		}
		else
		{
			USART_Printf("          \n\r");
		}
	}
}

