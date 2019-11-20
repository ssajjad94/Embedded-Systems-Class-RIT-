#include "game_task.h"

GAME_PARAMS_t game_params;

/*
	Task initizialization.
*/
void game_task_init(char *game_task_name)
{
	// Get pointer to THIS instance of parameters
	GAME_PARAMS_t *p = &game_params;   
	
	// Initialize task parameters
	strncpy(p->task_name, game_task_name, configMAX_TASK_NAME_LEN);
	p->servo_position = PWM_MINIMUM;
	
	// Create the task
  xTaskCreate(servo_computer_task, "USER COMMAND TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Task execution. 
*/
void game_task(void *parameters)
{
	GAME_PARAMS_t *p = &game_params;   
	
	StartLevel();
	
	while (1)
	{
		// Update the display
		//		(includes: score, time, player position, computer position
		uint16_t score = p->score;
		uint32_t elapsedTime = GetTimeElapsed();
		
		
		
		// Check if the player has time left (5 seconds counting down)
		if (IsPlayerOutOfTime(elapsedTime))
		{
			// Player loses the game
		}
		else
		{
		
			// Check if the player is within range of the computer
			
			// If so, reset
			StartLevel();
		}
	}
}


uint8_t IsPlayerOutOfTime(uint16_t curr_time)
{
	
}


uint16_t GetTimeElapsed(void)
{
	GAME_PARAMS_t *p = &game_params;
	
	uint32_t start_time = p->levelStartTime;
	uint32_t curr_time = TIM3->CNT;
	
	uint32_t elapsedTime = curr_time - start_time;
}


void StartLevel()
{
	GAME_PARAMS_t *p = &game_params;
	
	// Set the start time to now
	uint32_t start_time = TIM3->CNT;
	p->levelStartTime = start_time;
	
	// Set the computer to a random position
	GoToRandomPosition();
}