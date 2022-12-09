/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "utilities.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADC_BUF_LEN (2*768)
#define FULL_FLAG (1 << 4)
#define HALF_FLAG (1 << 0)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId blinkyTaskHandle;
osThreadId adcTaskHandle;
osThreadId rgbTaskHandle;
osThreadId lcdPrintTaskHandle;
osThreadId countingTaskHandle;
/* USER CODE BEGIN PV */
traceString ch0;
traceString adc_ch;
traceString lcd_ch;
struct TextLCDType hlcd;
float adc_val_pot;
float adc_val_sou;
SemaphoreHandle_t adcMutex;
char g_str_top[16];
char g_str_btm[16];
SemaphoreHandle_t lcdMutex;
uint16_t counter = 0;
SemaphoreHandle_t semButtonSemaphore;
uint16_t adc_buf[ADC_BUF_LEN];
EventGroupHandle_t xCreatedEventGroup;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void const * argument);
void StartBlinkyTask(void const * argument);
void StartAdcTask(void const * argument);
void StartRgbTask(void const * argument);
void StartLcdPrintTask(void const * argument);
void StartCountingTask(void const * argument);

/* USER CODE BEGIN PFP */
void setRGB(uint8_t r, uint8_t g, uint8_t b);
void swapChar(char** a, char** b);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == B1_Pin)
	{
		// Here goes your semaphore signaling code
		xSemaphoreGive(semButtonSemaphore);
	}
}
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  vTraceEnable(TRC_START);
  ch0 = xTraceRegisterString("ch0");
//  vTracePrintF(ch0, "I am tracing channel %d", 0);
  adc_ch = xTraceRegisterString("adc_ch");
  lcd_ch = xTraceRegisterString("lcd_ch");

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  setRGB(0xFF, 0xFF, 0xFF);

	HAL_TIM_Base_Start(&htim2);
	TextLCD_Init(&hlcd, &hi2c1, 0x4E, &htim2);
	sprintf(g_str_top, "Counter: %d", counter);

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  adcMutex = xSemaphoreCreateMutex();
  lcdMutex = xSemaphoreCreateMutex();
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  semButtonSemaphore = xSemaphoreCreateCounting(5, 0);
  xCreatedEventGroup = xEventGroupCreate();
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of blinkyTask */
  osThreadDef(blinkyTask, StartBlinkyTask, osPriorityAboveNormal, 0, 256);
  blinkyTaskHandle = osThreadCreate(osThread(blinkyTask), NULL);

  /* definition and creation of adcTask */
  osThreadDef(adcTask, StartAdcTask, osPriorityHigh, 0, 256);
  adcTaskHandle = osThreadCreate(osThread(adcTask), NULL);

  /* definition and creation of rgbTask */
  osThreadDef(rgbTask, StartRgbTask, osPriorityAboveNormal, 0, 256);
  rgbTaskHandle = osThreadCreate(osThread(rgbTask), NULL);

  /* definition and creation of lcdPrintTask */
  osThreadDef(lcdPrintTask, StartLcdPrintTask, osPriorityNormal, 0, 256);
  lcdPrintTaskHandle = osThreadCreate(osThread(lcdPrintTask), NULL);

  /* definition and creation of countingTask */
  osThreadDef(countingTask, StartCountingTask, osPriorityHigh, 0, 256);
  countingTaskHandle = osThreadCreate(osThread(countingTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  htim2.Init.Prescaler = 32-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
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
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4000-1;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_G_Pin|LED_R_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_G_Pin LED_R_Pin */
  GPIO_InitStruct.Pin = LED_G_Pin|LED_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_B_Pin */
  GPIO_InitStruct.Pin = LED_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_B_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void setRGB(uint8_t r, uint8_t g, uint8_t b)
{
	r = r ? GPIO_PIN_SET : GPIO_PIN_RESET; // ternary
	g = g ? GPIO_PIN_SET : GPIO_PIN_RESET;
	b = b ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, r);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, g);
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, b);
}

void swapChar(char** a, char** b)
{
	char *temp = *a;
	*a = *b;
	*b = temp;
}

// Called when first half of buffer is filled
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	vTracePrint(ch0, "HAL_ADC_ConvHalfCpltCallback");
	xEventGroupSetBits(xCreatedEventGroup, HALF_FLAG);

}
// Called when buffer is completely filled
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	vTracePrint(ch0, "HAL_ADC_ConvCpltCallback");
	xEventGroupSetBits(xCreatedEventGroup, FULL_FLAG);
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  osDelay(99999);
  /* Infinite loop */
  for(;;)
  {
	  osDelay(99999);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartBlinkyTask */
/**
* @brief Function implementing the blinkyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlinkyTask */
void StartBlinkyTask(void const * argument)
{
  /* USER CODE BEGIN StartBlinkyTask */
	TickType_t now = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  vTaskDelayUntil(&now , 500);
  }
  /* USER CODE END StartBlinkyTask */
}

/* USER CODE BEGIN Header_StartAdcTask */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAdcTask */
void StartAdcTask(void const * argument)
{
  /* USER CODE BEGIN StartAdcTask */
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_BUF_LEN); // Two Channels interleaved
	HAL_TIM_Base_Start(&htim3);
	TickType_t now = xTaskGetTickCount();
	EventBits_t uxBits;
	float adc_val_pot_temp = 0;
	float adc_val_sou_temp = 0;

  /* Infinite loop */
  for(;;)
  {
	  vTaskDelayUntil(&now, 50);

	  uxBits = xEventGroupWaitBits(
					xCreatedEventGroup,
				  HALF_FLAG | FULL_FLAG,
				  pdTRUE,
				  pdFALSE,
				  10000);

		if((uxBits & HALF_FLAG) != 0)
		{
			adc_val_pot_temp = averageSamples(&adc_buf[0], ADC_BUF_LEN / 4, 2);
			adc_val_sou_temp = soundLevel(&adc_buf[1], ADC_BUF_LEN / 4, 2);
		}
		else if((uxBits & FULL_FLAG)  != 0 )
		{
			adc_val_pot_temp = averageSamples(&adc_buf[768], ADC_BUF_LEN / 4, 2);
			adc_val_sou_temp = soundLevel(&adc_buf[769], ADC_BUF_LEN / 4, 2);
		}

		if( xSemaphoreTake( adcMutex, ( TickType_t ) 1000 ) == pdTRUE )
		{
			adc_val_pot = adc_val_pot_temp;
			adc_val_sou = adc_val_sou_temp;
			xSemaphoreGive( adcMutex );
		}


		if( xSemaphoreTake( lcdMutex, ( TickType_t ) 1000 ) == pdTRUE )
		{
			sprintf(g_str_btm, "%04.1f, %.1f", adc_val_pot, adc_val_sou);
			xSemaphoreGive( lcdMutex );
		}



	 // HAL_ADC_Start(&hadc1);
	 // HAL_ADC_PollForConversion(&hadc1, 100);
	 // adc_reading = HAL_ADC_GetValue(&hadc1);
	 // HAL_ADC_Stop(&hadc1);
	 // vTracePrintF(adc_ch, "ADC value: %d", adc_reading);
	 // sprintf(g_str_btm, "ADC value: %04d", adc_reading);
  }
  /* USER CODE END StartAdcTask */
}

/* USER CODE BEGIN Header_StartRgbTask */
/**
* @brief Function implementing the rgbTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRgbTask */
void StartRgbTask(void const * argument)
{
  /* USER CODE BEGIN StartRgbTask */
  /* Infinite loop */

	uint16_t adc_value;
	TickType_t last_blue_tick = 0;
	TickType_t last_red_tick = 0;
	TickType_t now = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&now, 30);

		if (adcMutex != NULL)
		{
			if( xSemaphoreTake( adcMutex, ( TickType_t ) 1000 ) == pdTRUE )
			{
				adc_value = adc_val_pot;
				xSemaphoreGive( adcMutex );
			}
		}
		else
		{
//			vTracePrintF(adc_ch, "adc_value blocked");
		}

	 //red
	 if (adc_value > 3000)
	 {
		 setRGB(0x00, 0xFF, 0x00);
		 last_red_tick = now + 1400;
		 last_blue_tick = now + 600 + 1400;
	 }
	 //yellow
	 else if ((adc_value > 2000) && (now > last_blue_tick))
	 {
		 setRGB(0x00, 0xFF, 0xFF);
	 }
	 //green
	 else if ((adc_value > 1000) && (now > last_blue_tick))
	 {
		 setRGB(0x00, 0x00, 0xFF);
	 }
	 //off
	 else if ((adc_value < 1000) && (now > last_blue_tick))
	 {
		 setRGB(0x00, 0x00, 0x00);
	 }
	 //blue
	 else if (now > last_red_tick)
	 {
		 setRGB(0xFF, 0x00, 0x00);
	 }

  }
  /* USER CODE END StartRgbTask */
}

/* USER CODE BEGIN Header_StartLcdPrintTask */
/**
* @brief Function implementing the lcdPrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLcdPrintTask */
void StartLcdPrintTask(void const * argument)
{
  /* USER CODE BEGIN StartLcdPrintTask */
	char str_top[16];
	char str_btm[16];

	TickType_t now = xTaskGetTickCount();
	/* Infinite loop */
	for(;;)
	{
		if (lcdMutex != NULL)
		{
			if( xSemaphoreTake ( lcdMutex, ( TickType_t ) 1000 ) == pdTRUE )
			{
				for (int i = 0; i < 16; i++)
				{
					str_top[i] = g_str_top[i];
					str_btm[i] = g_str_btm[i];
				}
				xSemaphoreGive( lcdMutex );
			}
		}
		else
		{
//			vTracePrintF(lcd_ch, "LCD blocked by mutex");
		}

		TextLCD_Position(&hlcd, 0, 0); // marker 1st row
		TextLCD_Puts(&hlcd, str_top);
		TextLCD_Position(&hlcd, 0, 1); // marker 2nd row
		TextLCD_Puts(&hlcd, str_btm);

		vTaskDelayUntil(&now, 50);

//		vTracePrintF(lcd_ch, "LCD Trace");
	}
  /* USER CODE END StartLcdPrintTask */
}

/* USER CODE BEGIN Header_StartCountingTask */
/**
* @brief Function implementing the countingTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCountingTask */
void StartCountingTask(void const * argument)
{
  /* USER CODE BEGIN StartCountingTask */
	/* Infinite loop */
	for (;;)
	{
		if(semButtonSemaphore != NULL)
		{
			if(xSemaphoreTake(semButtonSemaphore, (TickType_t) 1000) == pdTRUE )
			{
				counter++;

				if( xSemaphoreTake( lcdMutex, ( TickType_t ) 1000 ) == pdTRUE )
				{
					sprintf(g_str_top, "Counter: %d", counter);
					xSemaphoreGive( lcdMutex );
				}
			}
		}
		osDelay(2000);
	}

  /* USER CODE END StartCountingTask */
}

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
  __disable_irq();
  while (1)
  {
  }
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
