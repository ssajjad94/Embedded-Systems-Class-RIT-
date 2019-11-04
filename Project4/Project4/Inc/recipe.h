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

/*
// Restart recipe
void RestartRecipe(struct RecipeThread* thread);

// Toggles whether or not the recipe is paused.
void TogglePauseRecipe(struct RecipeThread* thread, uint8_t pause);

// Continuous update loop to be called
void RunRecipe(struct RecipeThread* thread);
*/

#endif
