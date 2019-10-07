#ifndef USERCOMMAND_H
#define USERCOMMAND_H

#include "stm32l476xx.h"
#include "UART.h"
#include "recipe.h"
#include "PWM.h"

uint8_t CheckForInputs(void);

char GetServoOneChar(void);
char GetServoTwoChar(void);

void ClearInputBuffer(void);
void PerformCommand(struct RecipeThread* thread, char command);

#endif
