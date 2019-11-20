#ifndef SERVO_COMPUTER_TASK
#define SERVO_COMPUTER_TASK

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "PWM.h"
#include "usart.h"
#include "rng.h"

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];

	// Task parameters
	int servo_id; 
	
	// Task variables
	uint8_t servo_position; 
	
} SERVO_COMPUTER_PARAMS_t;

extern SERVO_COMPUTER_PARAMS_t servo_computer_params;

void servo_computer_task(void *parameters);
void servo_computer_task_init(char *servo_computer_task_name, int servo_id);


// Set servo_position to a random position.
void GoToRandomPosition(void);



#endif
