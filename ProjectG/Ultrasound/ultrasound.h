/*
	ultrasound.h
	David Lin
*/


#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "timer.h"
#include "GPIO.h"
#include "uart.h"

// Timeout time -> 2 ms * 25 -> 0.05 seconds -> 50 ms
#define TIMEOUT_TIME_IN_US		(2000)
#define TIMOUT_MAX_ITER				(25)

// Max signal time
#define MAX_ECHO_TIME					(38 * MILLISECOND)

// The time for the internal signal to generate 8 pulses at 40kHz. 
#define SIGNAL_PERIOD (8 * 25 * MICROSECOND)

// The time for the echo to driiift away~
#define CYCLE_PERIOD (50 * MILLISECOND)

void InitUltrasound(void);

float ConvertToMilimeters(uint32_t distance_in_us);

uint32_t GetUltrasoundMeasurement(void);

#endif

