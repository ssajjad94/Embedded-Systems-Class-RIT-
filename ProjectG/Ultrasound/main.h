/*
	main.h
	David Lin
*/


#ifndef MAIN_H
#define MAIN_H

#include "stm32l476xx.h"

#include <string.h>
#include <stdio.h>

#include "SysClock.h"

#include "ultrasound.h"
#include "LED.h"
#include "UART.h"
#include "timer.h"
#include "GPIO.h"

#define HEADERLINE	"==============================================================================\n\r"

// Range for valid measurements (in mm)
#define LOWER_RANGE_LIMIT		(50)
#define UPPER_RANGE_LIMIT		(1000)

#define POST_VALID_RANGE_MIN			(300)
#define POST_VALID_RANGE_MAX			(1000)

// How many measurements to make
#define MEASUREMENT_CNT			(100)

// Prototypes
void MainProgramLoop(void);
uint8_t POSTTest(void);

#endif
