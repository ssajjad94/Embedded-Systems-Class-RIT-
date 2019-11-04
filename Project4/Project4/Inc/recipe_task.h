#ifndef __recipe_task_H    // header files should include guards
#define __recipe_task_H

#include "stm32l476xx.h"
#include "cmsis_os.h"

#include "string.h"

#include "recipe.h"
#include "PWM.h"

typedef struct {
  uint8_t id; // 0 or 1
  TaskHandle_t handle; 
  char task_name[16];
	
	// Params that the servo needs
	uint8_t servo;
	
	// Run status
	uint8_t m_bRunningRecipe;
	uint8_t m_bPaused;
	
	uint16_t m_Index;
	char m_Recipie[MAX_RECIPE_LEN];

	// Variables for when a wait is in place.
	uint32_t m_WaitIterations;
	uint32_t m_WaitIterationsElapsed; 

	// Variables for when a loop command is given
	int16_t m_LoopIndex;
	int16_t m_LoopCount;
	int16_t m_LoopCountMax;
	
	// Variables for error tracking
	uint8_t m_CommandError;
	uint8_t m_NestedLoopError;
	
	// Variable for sweeping
	uint8_t m_InSweep;
	
} RECIPE_PARAMS_t;

extern RECIPE_PARAMS_t recipe_params[2];

void recipe_task(void *parameters);
void recipe_task_init(int id, char *recipe_task_name, char recipie[], uint16_t size);

// Restart recipe
void RestartRecipe(uint8_t id);

// Toggles whether or not the recipe is paused.
void TogglePauseRecipe(uint8_t id, uint8_t pause);

// Tries to load and run next recipe
void RunRecipe(uint8_t id);

#endif

