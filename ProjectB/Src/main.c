/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

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
DAC_HandleTypeDef hdac1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

enum SignalType signal_type = SINE;
double amplitude = 0;
double frequency = 0;

// Quick test script;
uint8_t mode = 0;
	// 0 - amplitude decreasing, frequency same (10Hz)
	// 1 - amplitude increasing, frequency same (10Hz)
	// 2 - amplitude same (max), frequency increasing
	// 3 - amplitude same (max), frequency decreasing
uint8_t bAmplitudeMode = 0; 	// 0 for NONE, 1 for decreasing, 2 for increasing
uint8_t bFrequencyMode = 0; 	// 0 for NONE, 1 for decreasing, 2 for increasing

uint32_t iter = 0;
uint32_t maxiter = 1000;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
	// float voltage = 2.2;
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
	
  /* USER CODE BEGIN 2 */
	
  MX_USART2_UART_Init();
	// __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	// Clear display
	USART_Printf("\033[2J");		// Clear terminal
	USART_Printf("\033[0;0H");	// Reset cursor to 0,0
	USART_Printf("Starting program...\n\r");
	
	
	
	signal_type = GIBBS_PHENOMENON;
	amplitude = 3.3;
	frequency = 10;
	
	
	/*
	// Quick test script;
	mode = 0;
		// 0 - amplitude decreasing, frequency same (10Hz)
		// 1 - amplitude increasing, frequency same (10Hz)
		// 2 - amplitude same (max), frequency increasing
		// 3 - amplitude same (max), frequency decreasing
	bAmplitudeMode = 0; 	// 0 for NONE, 1 for decreasing, 2 for increasing
	bFrequencyMode = 0; 	// 0 for NONE, 1 for decreasing, 2 for increasing
	
	uint32_t maxiter = 10000;
	*/
	
  while (1)
  {
		// USART_Printf("\033[2J");		// Clear terminal
		// USART_Printf("\033[0;0H");	// Reset cursor to 0,0
		
		/*
		continue;
		
		USART_Printf("Amplitude: %f V (%d percent)", amplitude, ((int) amplitude / 3.3));	// Reset cursor to 0,0
		USART_Printf("Frequency: %f Hz (%d percent)", frequency);	// Reset cursor to 0,0
		
		if (mode == 0)
		{
			bAmplitudeMode = 1;
			bFrequencyMode = 0;
		}
		else if (mode == 1)
		{
			bAmplitudeMode = 2;
			bFrequencyMode = 0;
		}
		else if (mode == 2)
		{
			bAmplitudeMode = 0;
			bFrequencyMode = 2;
		}
		else if (mode == 3)
		{
			bAmplitudeMode = 0;
			bFrequencyMode = 1;
		}
		
		for (int i = 0; i < maxiter; i++)
		{
			if (bAmplitudeMode == 1)
			{
				amplitude  = (3.3) - (3.3) * ( (float) i / (float) maxiter);
			}
			else if (bAmplitudeMode == 2)
			{
				amplitude  = (3.3) * ( (float) i / (float) maxiter);
			}
			
			if (bFrequencyMode == 1)
			{
				frequency  = (10000) - (990) * ( (float) i / (float) maxiter);
			}
			else if (bFrequencyMode == 2)
			{
				frequency  = (10) + (990) * ( (float) i / (float) maxiter);
			}
		}
		
		mode += 1;
		if (mode > 3)
		{
			mode = 0;
			if (signal_type == GIBBS_PHENOMENON)
				signal_type = RAMP;
			else if (signal_type == RAMP)
				signal_type = TRIANGLE;
			else if (signal_type == TRIANGLE)
				signal_type = SINE;
			else if (signal_type == SINE)
				signal_type = GIBBS_PHENOMENON;
		}
		*/
		
		
		
		
		
		
		
		
	
		//while (!(USART2->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
		// USART resets the RXNE flag automatically after reading DR
		//uint8_t rxChar[2];
		//rxChar[0] = (uint8_t)(USART2->RDR & 0xFF);
		//rxChar[1] = 0x00;
		
		//USART_Printf("Testing... \n\r");
		
		/*
		// send it to the DISCOVERY BOARD serial port (USART2)
		HAL_UART_Receive_IT(&huart2, (uint8_t *)buffer, strlen(buffer));
		HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 100);
		
		if (buffer[0] == 'R')
		{
			signal_type = RAMP;
		}
		else if (buffer[0] == 'T')
		{
			signal_type = TRIANGLE;
		}
		else if (buffer[0] == 'S')
		{
			signal_type = SINE;
		}
		else if (buffer[0] == 'A')
		{
			signal_type = GIBBS_PHENOMENON;
		}
		*/
		
		// USART_Printf(buffer);
		
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
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

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */
  /**DAC Initialization 
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /**DAC channel OUT2 config 
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 3999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 399;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

void UpdateSignal(uint32_t time)
{
	USART_Printf("%d\n\r", mode);
	// UpdateCommand();
	
	// Calc voltage from -1.65V to 1.65V depending on signal equation. 
	double voltage = 0;
	double time_in_sec = ((double) time) / 20000.0;
	
	// Limit amplitude to 3.3V
	if (amplitude > VOLTAGE_RANGE)
		amplitude = VOLTAGE_RANGE;
		
	if (signal_type == SINE)
	{
		// Calc sine signal
		double two_pi = 2.0 * 3.14;
		double half_amp = amplitude / 2.0;
		double t = time_in_sec * two_pi * frequency;
		
		// Calc voltage from negative half amp to positive half amp
		voltage = half_amp * sin(t);
		
		// Shift from (-half_amp to + half_amp) to (0 to amp)
		voltage += half_amp;
	}
	else if (signal_type == GIBBS_PHENOMENON)
	{
		// Calc sine signal
		double two_pi = 2.0 * 3.14;
		double half_amp = amplitude / 2.0;
		double t = time_in_sec * two_pi * frequency;
		
		// Calc voltage from negative half amp to positive half amp
		voltage = half_amp * ( sin(t) + (sin(3.0 * t) / 3.0) 
							+ (sin(5.0 * t) / 5.0) + (sin(7.0 * t) / 7.0) 
							+ (sin(9.0 * t) / 9.0) );
	}
	else if (signal_type == TRIANGLE)
	{
		double period = ((double) 1.0) / frequency;
		double sectional_time = fmod(time_in_sec, period);
		if (sectional_time < period/2)
		{
			voltage = amplitude - amplitude * ((sectional_time) / (period / 2.0));
		}
		else
		{
			voltage = amplitude * ((sectional_time) / (period / 2.0) - 1.0);
		}
	}
	else if (signal_type == RAMP)
	{
		double period = ((double) 1.0) / frequency;
		double sectional_time = fmod(time_in_sec, period);

		voltage = amplitude - amplitude * ((sectional_time) / (period));
	}
	
	// Convert value to 8-bit number
	uint8_t value = (uint8_t) ((voltage / VOLTAGE_RANGE) * 255.0);
	
	// Send it over to the DAC
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_8B_R, value);
}

void UpdateCommand()
{
	// USART_Printf("Amplitude: %f V (%d percent)", amplitude, ((int) amplitude / 3.3));	// Reset cursor to 0,0
	// USART_Printf("Frequency: %f Hz (%d percent)", frequency);	// Reset cursor to 0,0
		
	if (mode == 0)
	{
		bAmplitudeMode = 1;
		bFrequencyMode = 0;
	}
	else if (mode == 1)
	{
		bAmplitudeMode = 2;
		bFrequencyMode = 0;
	}
	else if (mode == 2)
	{
		bAmplitudeMode = 0;
		bFrequencyMode = 2;
	}
	else if (mode == 3)
	{
		bAmplitudeMode = 0;
		bFrequencyMode = 1;
	}
		
	iter += 1;
	float i = iter;
	if (bAmplitudeMode == 1)
	{
		amplitude  = (3.3) * (1.0 -  (i / ((float) maxiter)));
	}
	else if (bAmplitudeMode == 2)
	{
		amplitude  = (3.3) * ( i / ((float) maxiter));
	}
	else
	{
		amplitude = 3.3;
	}
	
	if (bFrequencyMode == 1)
	{
		frequency  = (100.0) - (99.0) * ( i / (float) maxiter);
	}
	else if (bFrequencyMode == 2)
	{
		frequency  = (1.0) + (99.0) * ( i / ((float) maxiter));
	}
	else
	{
		frequency = 100;
	}
	
	if (iter > maxiter)
	{
		iter = 0;
		
		mode += 1;
		if (mode > 3)
		{
			mode = 0;
			if (signal_type == GIBBS_PHENOMENON)
				signal_type = RAMP;
			else if (signal_type == RAMP)
				signal_type = TRIANGLE;
			else if (signal_type == TRIANGLE)
				signal_type = SINE;
			else if (signal_type == SINE)
				signal_type = GIBBS_PHENOMENON;
		}
	}
}


void USART_Printf(const char *fmt, ...) {
	char buffer[110];
	
	// create formatted print string
  // Note: va_list, va_start(), vsnprintf(), va_end() are all part of C standard library
	va_list argptr;
	va_start(argptr, fmt);
  // vsnprintf is kind of like snprintf with indirection to varag stack
	vsnprintf(buffer, sizeof(buffer), fmt, argptr);
	va_end(argptr);
	
	// send it to the DISCOVERY BOARD serial port (USART2)
	HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 100);
}


/* USER CODE END 4 */

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
