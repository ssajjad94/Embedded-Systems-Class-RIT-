/*
	ultrasound.h
	David Lin
*/


#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include "timer.h"
#include "GPIO.h"

#define TIMEOUT_TIME_IN_US		(3000)

void InitUltrasound(void);

uint32_t GetUltrasoundMeasurement(void);

#endif

