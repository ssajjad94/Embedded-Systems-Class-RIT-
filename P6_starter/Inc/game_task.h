#ifndef GAME_TASK
#define GAME_TASK

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "PWM.h"
#include "usart.h"
#include "main.h"
#include "servo_user_task.h"
#include "servo_computer_task.h"

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];

	// Task parameters
	float score; 
	uint32_t levelStartTime;
	
} GAME_PARAMS_t;

extern GAME_PARAMS_t game_params;

void game_task(void *parameters);
void game_task_init(char *game_task_name);


// Updates the display with score, time elapsed, player position, computer position
void UpdateDisplay(void);

// Checks if the player is out of time
uint8_t IsPlayerOutOfTime(uint16_t curr_time);

// Checks how much time has passed
uint16_t GetTimeElapsed(void);

//
void StartLevel(void);

//@todo add function to check player-computer relative position 


#endif
