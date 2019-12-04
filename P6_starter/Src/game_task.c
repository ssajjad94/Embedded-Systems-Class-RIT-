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
	p->score = 0;
	p->levelNum = 0;
	
	// Create the task
  xTaskCreate(game_task, "GAME TASK", 256, (void *)p, 2, &p->handle);
}



/*
	Task execution. 
*/
void game_task(void *parameters)
{
	GAME_PARAMS_t *p = &game_params;   
	
	RestartGame();
	
	while (1)
	{
		uint16_t score = p->score;
		p->elapsedTime = GetTimeElapsed();
		
		uint8_t playerPos = (&servo_user_params)->servo_position;
		uint8_t computerPos = (&servo_computer_params)->servo_position;
		
		// Check if the player has time left (5 seconds counting down)
		if (p->elapsedTime > MAX_PLAYER_TIME_SEC)// && p->elapsedTime > 0 && p->elapsedTime < UNREASONABLY_LARGE_TIME)
		{
			// Player loses the game
			p->gameoverState = 1;
	
			// Delay to let it sink in...
			WaitTIM3(TICKS_PER_SECOND * 1);
			
			// Restart
			RestartGame();
		}
		else
		{
			// Reinforce the game is not over
			p->gameoverState = 0;
			
			// Check if the player is within range of the computer
			int deltaPos = playerPos - computerPos;
			if (deltaPos < 0)
				deltaPos = deltaPos * (-1);
			
			
			// If so....
			if (deltaPos < (.05)*(PWM_MAXIMUM - PWM_MINIMUM))
			{
				// Increment score
				p->score += ((float) (MAX_PLAYER_TIME_SEC - p->elapsedTime)) / TICKS_PER_SECOND;
				
				// Start next level
				StartLevel();
			}
		}
	}
}


uint32_t GetTimeElapsed(void)
{
	GAME_PARAMS_t *p = &game_params;
	
	uint32_t start_time = p->levelStartTime;
	uint32_t curr_time = TIM3->CNT;
	
	uint32_t elapsedTime = curr_time - start_time;
	
	//if (elapsedTime > 1073741824)	// Invalid number
		// return elapsedTime + ;
	
	return elapsedTime;
}


void RestartGame(void)
{
	GAME_PARAMS_t *p = &game_params;
	
	// Reset score and level
	p->levelNum = 0;
	p->score = 0;
	p->gameoverState = 0;
	
	// Start the first level
	StartLevel();
}


void StartLevel()
{
	GAME_PARAMS_t *p = &game_params;
	
	// Set the start time to now
	TIM3->CNT = 0;
	p->levelStartTime = TIM3->CNT;
	p->levelNum += 1;
	
	// Set the computer to a random position
	GoToRandomPosition();
}

