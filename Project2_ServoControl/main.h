#ifndef MAIN_H
#define MAIN_H

#include "stm32l476xx.h"

#include <string.h>
#include <stdio.h>

#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "timer.h"
#include "GPIO.h"
#include "recipe.h"

#define PRINT(x)					USART_Write(USART2, (uint8_t*)x)
#define PRINT_UINT16(x)		USART_WriteUInt16(USART2, x);

#define HEADERLINE	"==============================================================================\n\r"

uint8_t PromptYesOrNo(uint8_t* prompt);

uint8_t PerformPOST(void);

uint16_t PromptForLimit(uint16_t minLimit, uint16_t maxLimit);

#endif
