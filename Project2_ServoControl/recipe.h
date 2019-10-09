#ifndef RECIPE_H
#define RECIPE_H

#include "stm32l476xx.h"
#include "PWM.h"


#define OPCODE_MASK (0xE0)
#define OPCODE_SHIFT (5)

#define PARAM_MASK (0x1F)

#define OP_MOV 				(0x01)
#define OP_WAIT 			(0x02)
#define OP_PAUSE			(0x03)
#define OP_LOOP 			(0x04)
#define OP_ENDLOOP 		(0x05)
#define OP_SWEEP			(0x07)
#define OP_RECIPEEND 	(0x00)

#define MAX_RECIPE_LEN	(100)

#define WAIT_ITER_AFTER_MOV (5)

struct RecipeThread 
{
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
};

// Loads a new recipe and inits variables
void LoadNewRecipe(struct RecipeThread* thread, char recipie[], uint16_t size);

// Restart recipe
void RestartRecipe(struct RecipeThread* thread);

// Toggles whether or not the recipe is paused.
void TogglePauseRecipe(struct RecipeThread* thread, uint8_t pause);

// Continuous update loop to be called
void RunRecipe(struct RecipeThread* thread);


#endif
