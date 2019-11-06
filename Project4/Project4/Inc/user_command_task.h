#ifndef USERCOMMAND_H
#define USERCOMMAND_H

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include "PWM.h"
#include "usart.h"

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];
	
	// InputBuffer
	char inputBuffer[3];
	
} USER_COMMAND_PARAMS_t;

extern USER_COMMAND_PARAMS_t user_command_params;

void user_command_task(void *parameters);
void user_command_task_init(char *user_command_task_name);

// Checks if the buffer was popped after being full. If so, empty.
void CheckToClearBuffer(void);

// Checks for user input and pushes it to a buffer.
void PollForInputs(void);

// Public -> should be called by Recipe Task
char PopServoChar(uint8_t servo);

#endif
