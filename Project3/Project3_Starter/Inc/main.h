/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "Customer.h"
#include "Teller.h"
#include "Break.h"
#include "TaskUpdate.h"
#include "TaskTeller.h"
#include "TaskCustomerEnqueue.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void DisplayMetrics(void);

uint8_t IsBankOpen(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

// Macros to define time
#define T_SECOND	(1)
#define T_MINUTE	(60 * T_SECOND)
#define T_HOUR 		(60 * T_MINUTE)

// Macro to define simulated time based on real time
	// 100ms = 1min = 60s
	// 1s = 600s
#define REAL_TO_SIMULATED_RATIO	(600)


// Bank hours
#define BANK_START_TIME 	(0) 						// 0s after 9am
#define BANK_CLOSE_TIME 	(7 * T_HOUR)		// 25200s (7 hours) after 9am (4pm)


// Customer queue and list
#define CUSTOMER_QUEUE_LENGTH		(100)
#define CUSTOMER_LIST_LENGTH		(421)
	// Max number of custoemrs comes from 7 hours, worst cast 1 customer per minute

#define ADD_CUSTOMER_MIN_TIME		(1 * T_MINUTE)
#define ADD_CUSTOMER_MAX_TIME		(4 * T_MINUTE)

// Teller's individual list of customers
#define TELLER_LIST_OF_CUSTOMERS_LENGTH	(421)
#define TELLER_LIST_OF_BREAKS_LENGTH		(15)

// Range for random customer service time
#define MIN_CUSTOMER_SERVICE_TIME	(30 * T_SECOND)
#define MAX_CUSTOMER_SERVICE_TIME	(8 * T_MINUTE)

// Range for random break time
#define MIN_BREAK_TIME				(1 * T_MINUTE)
#define MAX_BREAK_TIME				(4 * T_MINUTE)

// Range for scheduling next break time
#define MIN_NEXT_BREAK_TIME		(30 * T_MINUTE)
#define MAX_NEXT_BREAK_TIME		(60 * T_MINUTE)


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
