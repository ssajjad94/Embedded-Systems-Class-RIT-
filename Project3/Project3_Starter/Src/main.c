/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "rng.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led_task.h"           // needed for led_task_init
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

	// Create the array of tellers (size three)
	Teller tellers[3];

	// Simulation Clock
	uint32_t simulation_clock;

	// Customer queue
	QueueHandle_t customer_queue;

	// Create three customer list arrays
	Customer teller0_customer_list[TELLER_LIST_OF_CUSTOMERS_LENGTH];
	Customer teller1_customer_list[TELLER_LIST_OF_CUSTOMERS_LENGTH];
	Customer teller2_customer_list[TELLER_LIST_OF_CUSTOMERS_LENGTH];

	// Create three break list arrays 
	Break teller0_break_list[TELLER_LIST_OF_BREAKS_LENGTH];
	Break teller1_break_list[TELLER_LIST_OF_BREAKS_LENGTH];
	Break teller2_break_list[TELLER_LIST_OF_BREAKS_LENGTH];
	
	// Next customer time
	uint32_t next_customer_time;
	
	// All customers
	Customer all_customers[CUSTOMER_LIST_LENGTH];
	uint32_t all_customers_cnt;
	uint32_t max_customer_queue_cnt;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	// Simulation Clock
	simulation_clock = 0;
	
	// All customers
	all_customers_cnt = 0;
	max_customer_queue_cnt = 0;
	
	// Initialize the queue
	customer_queue = xQueueCreate(CUSTOMER_QUEUE_LENGTH, sizeof(Customer));
	if ( customer_queue == NULL )
	{
		USART_Printf("Failed to create queue\n\r");
		
		while(1){;;}
	}
	
	// For each teller, initialize IDLE and array counts to 0
	for (int i = 0; i < 3; i++)
	{
		tellers[i].status = Idle;
		tellers[i].serviced_customers_cnt = 0;
		tellers[i].break_cnt = 0;
	}
	
	// Assign a customer list array to each teller
	tellers[0].serviced_customers = teller0_customer_list;
	tellers[1].serviced_customers = teller1_customer_list;
	tellers[2].serviced_customers = teller2_customer_list;
	
	// Assign a break list array to each teller
	tellers[0].breaks = teller0_break_list;
	tellers[1].breaks = teller1_break_list;
	tellers[2].breaks = teller2_break_list;
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RNG_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	
	// Customer enqueue time 
	// 	Default to 1-4 minutes after the bank opens
	next_customer_time = 0;
	{
		// Calculate a random time from 1 to 4 minutes
		uint32_t random_time;
		uint32_t min_time = MIN_BREAK_TIME;
		uint32_t max_time = MAX_BREAK_TIME;
		HAL_RNG_GenerateRandomNumber(&hrng, &random_time);
		random_time = min_time + (random_time % (max_time - min_time));
		
		next_customer_time = random_time;
	}
	
	// Plan initial breaks for each teller
	for (int i = 0; i < 3; i++)
	{
		// Calculate a random time from 30 to 60 minutes
		uint32_t random_time;
		uint32_t min_time = MIN_NEXT_BREAK_TIME;
		uint32_t max_time = MAX_NEXT_BREAK_TIME;
		HAL_RNG_GenerateRandomNumber(&hrng, &random_time);
		random_time = min_time + (random_time % (max_time - min_time));
		
		tellers[i].breaks[0].start_time = random_time;
		tellers[i].breaks[0].end_time = 0;
		tellers[i].break_cnt = 1;
	}
	
	
	
	// Start the update task
	update_task_init("UPDATE_TASK", &simulation_clock, &customer_queue, tellers);

	// Start the teller task
	teller_service_task_init("TELLER TASK 0", 0, &simulation_clock, &(tellers[0]), &customer_queue);
	teller_service_task_init("TELLER TASK 1", 1, &simulation_clock, &(tellers[1]), &customer_queue);
	teller_service_task_init("TELLER TASK 2", 2, &simulation_clock, &(tellers[2]), &customer_queue);

	// Customer enqueue task
	customer_enqueue_task_init("CUSTOMER ENQUEUE TASK", &simulation_clock, all_customers,
		&all_customers_cnt, &customer_queue, &max_customer_queue_cnt, &next_customer_time);

  // USART_Printf is printf() customized to this platform and uses a variable argugment list.
  // It is convenient but unnecessary.  You can use HAL functions (e.g. HAL_USART_Transmit())
  USART_Printf("System initialized, starting FreeRTOS\r\n");
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RNG;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void DisplayMetrics()
{
	// Print metrics
	
	// Print the total number of customers served during the day
	USART_Printf("Total number of customers: %d \n\r", all_customers_cnt);
	
	// The number of customers served by each teller
	USART_Printf("Teller 0's number of customers: %d\n\r", tellers[0].serviced_customers_cnt);
	USART_Printf("Teller 1's number of customers: %d\n\r", tellers[1].serviced_customers_cnt);
	USART_Printf("Teller 2's number of customers: %d\n\r", tellers[2].serviced_customers_cnt);

	// The average time each customer spends waiting in the queue.
	uint32_t maxTimeCustomerWaitingInQueue = 0;
	uint32_t totalTimeCustomerWaitingInQueue = 0;
	for (int i = 0; i < all_customers_cnt; i++)
	{
		// The time each customer spends waiting in the queue is the
		//		difference between their service start time and arrival time
		uint32_t timeWaiting = all_customers[i].service_start_time - all_customers[i].arrival_time;
		
		totalTimeCustomerWaitingInQueue += timeWaiting;
		
		if (timeWaiting > maxTimeCustomerWaitingInQueue)
			maxTimeCustomerWaitingInQueue = timeWaiting;
	}
	uint32_t avgTimeCustomerWaitingInQueue = totalTimeCustomerWaitingInQueue / all_customers_cnt;
	USART_Printf("Average time customer spends waiting "); 
	USART_Printf("in the queue: %d seconds\n\r", avgTimeCustomerWaitingInQueue);
	
	// The average time each customer spends with the teller.
	uint32_t totalTimeCustomerServiced = 0;
	for (int i = 0; i < all_customers_cnt; i++)
	{
		// The time each customer spends waiting in the queue is the
		//		difference between their service start time and arrival time
		totalTimeCustomerServiced = all_customers[i].service_end_time - all_customers[i].service_start_time;
	}
	uint32_t avgTimeCustomerServiced = totalTimeCustomerServiced / all_customers_cnt;
	USART_Printf("Average time customer spends ");
	USART_Printf("with the teller: %d seconds\n\r", avgTimeCustomerServiced);
	
	// The average time tellers wait for customers.
	uint32_t maxTimeWaitingForCustomer = 0;
	uint32_t totalTimeWaitingForCustomer = 0;
	for (int id = 0; id < 3; id++)
	{
		Teller teller = tellers[id];
		
		if (teller.serviced_customers_cnt > 0)
		{
			// The first customer's wait time is from 0 to their service start time
			uint32_t firstCustomerWait = teller.serviced_customers[0].service_start_time;
			totalTimeWaitingForCustomer += firstCustomerWait;
			
			if (firstCustomerWait > maxTimeWaitingForCustomer)
					maxTimeWaitingForCustomer = firstCustomerWait;
				
			for (int i = 1; i < teller.serviced_customers_cnt; i++)
			{
				// Subsequent customer's wait time is from previous's end time to 
				//		next start time
				uint32_t timeWaiting = teller.serviced_customers[i].service_start_time 
					- teller.serviced_customers[i-1].service_end_time ;
				totalTimeWaitingForCustomer += timeWaiting;
				
				if (timeWaiting > maxTimeWaitingForCustomer)
					maxTimeWaitingForCustomer = timeWaiting;
			}
		}
	}
	uint32_t avgTimeWaitingForCustomer = totalTimeWaitingForCustomer / all_customers_cnt;
	USART_Printf("Average time teller waits for ");
	USART_Printf("customers: %d seconds\n\r", avgTimeWaitingForCustomer);

	// The maximum customer wait time in the queue. 
	USART_Printf("Maximum time customer spends ");
	USART_Printf("waiting in the queue: %d seconds\n\r", maxTimeCustomerWaitingInQueue);
	
	// The maximum wait time for tellers waiting for customers. 
	USART_Printf("Maximum time teller waits ");
	USART_Printf("for customers: %d seconds\n\r", maxTimeWaitingForCustomer);
	
	// The maximum transaction time for the tellers.
	for (int id = 0; id < 3; id++)
	{
		Teller teller = tellers[id];
		
		uint32_t maxBusyTime = 0;
		for (int i = 0; i < teller.serviced_customers_cnt; i++)
		{
			// The busy time is the difference between end service time and start service time
			uint32_t busyTime = teller.serviced_customers[i].service_end_time - teller.serviced_customers[i].service_start_time;
			
			if (busyTime > maxBusyTime)
				maxBusyTime = busyTime;
		}
		
		USART_Printf("Teller %d's maximum transaction ");
		USART_Printf("time: %d seconds\n\r", id, maxBusyTime);
	}
	
	// The maximum depth of the customer queue.
	USART_Printf("Maximum depth of the customer ");
	USART_Printf("queue: %d\n\r", max_customer_queue_cnt);

	for (int id = 0; id < 3; id++)
	{
		Teller teller = tellers[id];
		
		uint32_t totalBreakTime = 0;
		uint32_t minBreakTime = 0;
		uint32_t maxBreakTime = 0;
		
		for (int i = 0; i < teller.break_cnt; i++)
		{
			// Break time is the difference between the break's end time and start time
			uint32_t break_time = teller.breaks[i].end_time - teller.breaks[i].start_time;
			
			totalBreakTime += break_time;
			
			// Update max break time
			if (break_time > maxBreakTime)
			{
				maxBreakTime = break_time;
			}
			
			// Update min break time
			if (i == 0)
			{
				// First break -> auto set to minimum
				minBreakTime = break_time;
			}
			else
			{
				// Subsequent breaks -> compare to minimum
				if (break_time < minBreakTime)
				{
					minBreakTime = break_time;
				}
			}
		}
		
		uint32_t avgBreakTime = totalBreakTime / teller.break_cnt;
		
		// Number of breaks for each of the three tellers
		USART_Printf("Teller %d's number of breaks: %d\n\r", id, teller.break_cnt);

		// Average break time for each of the three tellers
		USART_Printf("Teller %d's average break time: %d\n\r", id, avgBreakTime);

		// Longest break time for each of the three tellers
		USART_Printf("Teller %d's longest break time: %d\n\r", id, maxBreakTime);

		// Shortest break time for each of the three tellers
		USART_Printf("Teller %d's shortest break time: %d\n\r", id, minBreakTime);
	}

}

/**
	* Helper function to check if the bank is open
	*/
uint8_t IsBankOpen(void)
{
	if (simulation_clock >= BANK_START_TIME && simulation_clock <= BANK_CLOSE_TIME)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
