#ifndef SERVO_USER_TASK
#define SERVO_USER_TASK

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "PWM.h"
#include "usart.h"
#include "main.h"

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];

	// Task parameters
	int servo_id; 
	
	// Task variables
	uint8_t servo_position; 
	
} SERVO_USER_PARAMS_t;

extern SERVO_USER_PARAMS_t servo_user_params;

void servo_user_task(void *parameters);
void servo_user_task_init(char *servo_user_task_name, int servo_id);


// Set servo_position to a random position.
void GoToAnotherPosition(void);

// Uses the gyro to get the position;
uint8_t CalcPosition(void);

#endif
