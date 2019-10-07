#ifndef MAIN_H
#define MAIN_H

#include "stm32l476xx.h"

#include <string.h>
#include <stdio.h>

#include "SysClock.h"
#include "UART.h"

#include "recipe.h"
#include "PWM.h"
#include "LED.h"
#include "UserCommand.h"
#include "timer.h"

#define PRINT(x)					USART_Write(USART2, (uint8_t*)x)
#define PRINT_UINT16(x)		USART_WriteUInt16(USART2, x);

#define HEADERLINE	"==============================================================================\n\r"

uint8_t PromptYesOrNo(uint8_t* prompt);


char recipie_test[] = 
{
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_SWEEP << OPCODE_SHIFT),
	(OP_MOV << OPCODE_SHIFT) | 5,
	(OP_MOV << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_LOOP << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_ENDLOOP << OPCODE_SHIFT),
	(OP_MOV << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 2,
	(OP_WAIT << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_WAIT << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 2,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_WAIT << OPCODE_SHIFT) | 31,
	(OP_WAIT << OPCODE_SHIFT) | 31,
	(OP_WAIT << OPCODE_SHIFT) | 31,
	(OP_MOV << OPCODE_SHIFT) | 4,
	(OP_RECIPEEND << OPCODE_SHIFT) 
};

#endif
