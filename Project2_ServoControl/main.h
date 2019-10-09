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
#define ENTER_COMMAND "PLEASE ENTER A COMMAND: "

uint8_t PromptYesOrNo(uint8_t* prompt);


// This recipe demonstrates the desired test recipe from the lab exercise description
char recipie_test_0[] = 
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

// This recipe demonstrates moving to every position
char recipie_test_1[] = 
{
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 2,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_MOV << OPCODE_SHIFT) | 4,
	(OP_MOV << OPCODE_SHIFT) | 5,
	(OP_WAIT << OPCODE_SHIFT) | 15,	//1.6 sec
	(OP_SWEEP << OPCODE_SHIFT),
	(OP_WAIT << OPCODE_SHIFT) | 15,	//1.6 sec
	(OP_MOV << OPCODE_SHIFT) | 5,
	(OP_MOV << OPCODE_SHIFT) | 4,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_MOV << OPCODE_SHIFT) | 2,
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 0,
	(OP_RECIPEEND << OPCODE_SHIFT) 
};

// This recipe demonstrates a mov after the recipeend command.
//		The last mov should NOT execute, even after running continue.
char recipie_test_2[] = 
{
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 2,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_MOV << OPCODE_SHIFT) | 4,
	(OP_MOV << OPCODE_SHIFT) | 5,
	
	(OP_RECIPEEND << OPCODE_SHIFT), 
	
	(OP_MOV << OPCODE_SHIFT) | 1
};


// This recipe demonstrates a nested-loop error. 
//		The recipe should NOT continue after the error occurs.
char recipie_test_3[] = 
{
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 5,
	(OP_LOOP << OPCODE_SHIFT) | 0,
	(OP_LOOP << OPCODE_SHIFT) | 0,
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_MOV << OPCODE_SHIFT) | 3,
	(OP_ENDLOOP << OPCODE_SHIFT),
	(OP_MOV << OPCODE_SHIFT) | 4,
	(OP_MOV << OPCODE_SHIFT) | 5,
	(OP_RECIPEEND << OPCODE_SHIFT)
};

// This recipe demonstrates the grad extensions, SWEEP and PAUSE
char recipie_test_4[] = 
{
	(OP_MOV << OPCODE_SHIFT) | 1,
	(OP_WAIT << OPCODE_SHIFT) | 9,	//1.0 sec
	(OP_SWEEP << OPCODE_SHIFT),
	(OP_PAUSE << OPCODE_SHIFT),
	(OP_SWEEP << OPCODE_SHIFT),
	(OP_RECIPEEND << OPCODE_SHIFT)
};


#endif
