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

#define PRINT(x)					USART_Write(USART2, (uint8_t*)x)
#define PRINT_UINT16(x)		USART_WriteUInt16(USART2, x);

#define HEADERLINE	"==============================================================================\n\r"

// A pulse must be seen under POST_TIME_MS ms
#define POST_TIME_MS		(100)

// A pulse must be seen under POST_TIME_US us
#define POST_TIME_US    (POST_TIME_MS * 1000)



// The default value of the lower limit
#define LOWER_LIMIT_DEFAULT		(950)

// The valid range for the lower limit, inclusive
#define LOWER_LIMIT_MIN				(50)
#define	LOWER_LIMIT_MAX				(9950)

uint8_t PromptYesOrNo(uint8_t* prompt);
uint8_t PerformPOST(void);
uint16_t PromptForLimit(uint16_t minLimit, uint16_t maxLimit);

#endif
