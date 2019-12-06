#ifndef DISPLAY_TASK
#define DISPLAY_TASK

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "usart.h"
#include "timer.h"

#include "servo_computer_task.h"
#include "servo_user_task.h"
#include "game_task.h"

typedef struct {
  TaskHandle_t handle; 
  char task_name[16];

} DISPLAY_TASK_PARAMS_t;

extern DISPLAY_TASK_PARAMS_t display_task_params;

void display_task(void *parameters);
void display_task_init(char *display_task_name);

#endif

