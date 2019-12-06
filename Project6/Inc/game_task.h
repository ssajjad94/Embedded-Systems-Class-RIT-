#ifndef GAME_TASK
#define GAME_TASK

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "PWM.h"
#include "usart.h"
#include "timer.h"
#include "main.h"

#include "servo_user_task.h"
#include "servo_computer_task.h"

#define TICKS_PER_SECOND				(10000)
#define MAX_PLAYER_TIME_SEC			(5 * TICKS_PER_SECOND)
#define UNREASONABLY_LARGE_TIME	(15 * TICKS_PER_SECOND)

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];

	// Task parameters
	float score; 
	uint32_t levelNum;
	uint32_t levelStartTime;
	uint32_t elapsedTime;
	uint8_t gameoverState;
	
} GAME_PARAMS_t;

extern GAME_PARAMS_t game_params;

void game_task(void *parameters);
void game_task_init(char *game_task_name);


// Updates the display with score, time elapsed, player position, computer position
void UpdateDisplay(void);

// Checks how much time has passed
uint32_t GetTimeElapsed(void);

//
void StartLevel(void);

// 
void RestartGame(void);

//@todo add function to check player-computer relative position 


#endif
