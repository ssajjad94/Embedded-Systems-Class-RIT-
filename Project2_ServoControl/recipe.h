#ifndef RECIPE_H
#define RECIPE_H

#include "stm32l476xx.h"
#include "PWM.h"


#define OPCODE_MASK (0b11100000)
#define OPCODE_SHIFT (5)

#define PARAM_MASK (0b00011111)

#define OP_MOV 				(0b001)
#define OP_WAIT 			(0b010)
#define OP_LOOP 			(0b100)
#define OP_ENDLOOP 		(0b101)
#define OP_RECIPEEND 	(0b000)

struct RecipeThread 
{
	uint8_t servo;
	
	// Run status
	uint8_t m_bRunningRecipe;
	uint8_t m_bPaused;
	
	uint16_t m_Index;
	char* m_Recipie;

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
};

// Loads a new recipe and inits variables
void LoadNewRecipe(struct RecipeThread* thread, char recipie[]);

// Toggles whether or not the recipe is paused.
void TogglePauseRecipe(struct RecipeThread* thread, uint8_t pause);

// Continuous update loop to be called
void RunRecipe(struct RecipeThread* thread);

#endif
