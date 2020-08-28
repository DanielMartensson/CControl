/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "LCD_ILI9341/LCDCore/LCD_ILI9341.h"
#include "LCD_ILI9341/LCDTools/LCDTools.h"
#include "CControl/Headers/Functions.h"
#include "FlashMemory/ReadWrite.h"
#include "Encoder/Encoder.h"
#include "MCP2515/j1939.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RDIM 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Definitions for LCDTask */
osThreadId_t LCDTaskHandle;
const osThreadAttr_t LCDTask_attributes = {
  .name = "LCDTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 300 * 4
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 300 * 4
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM5_Init(void);
static void MX_USART2_UART_Init(void);
void StartLCDTask(void *argument);
void StartControlTask(void *argument);

/* USER CODE BEGIN PFP */
void readFromMCP2515();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

enum frame{
	MRAC_FRAME,
	J1939_FRAME
};

enum screen{
	PLOT,
	ADDRESS,
	CONFIGURATION,
	DM1_MESSAGES
};

/* Settings */
float mrac_gain;
float mrac_saturation;
float valve_delay;
const float one_rotation_pulses = 30;
bool closedloop_on = true;

/* Measurements */
volatile uint16_t adcData[2];
Encoder encoder;
uint8_t UART_RX_DATA[1]; // For 0x23 command
uint8_t UART_TX_DATA[16];
Convert convert; // We borrow the convert Union from folder FlashMemory -> ReadWrite.h file
float new_input0_float;
float new_input1_float;
float new_output_float;
float new_reference_float;

/* LCD */
ILI9341_SPI lcd_spi;

/* J1939 */
J1939 j1939;
bool rename_CA_SA = false;
bool send_a_claim_address_request = false;
bool reset_DM1_messages = false;
bool send_a_configuration_request_read = false;
bool send_a_configuration_request_write = false;

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	// Start encoder
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	Encoder_init(&encoder, &htim5, one_rotation_pulses);

	// Start ADC with DMA
	HAL_TIM_Base_Start(&htim2);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcData, 2);

	// Start DMA for UART
	HAL_UART_Receive_DMA(&huart2, UART_RX_DATA, 1);

	// Set the address and sector for F401RE CPU where we are going to store the settings. Look at chapter 3.3 at document RM0368 for STM32F401xB/C and STM32F401xD/E processors
	FlashSetSectorAddrs(7, 0x08060000);

	// Init LCD
	ILI9341_Init(&lcd_spi, &hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin, TS_CS_GPIO_Port, TS_CS_Pin);

	// Show welcome screen
	ILI9341_setRotation(&lcd_spi, 2);
	ILI9341_printImage(&lcd_spi, 0, 0, 320, 240, windowsME_intro, 640 * 240 * sizeof(uint8_t));
	HAL_Delay(5000);

	// Load the user configurations
	float rdBuf[7];
	FlashReadN(0, rdBuf, 7, DATA_TYPE_FLOAT);
	lcd_spi.myTS_Calibrate.Scale_X = rdBuf[0];
	lcd_spi.myTS_Calibrate.Scale_Y = rdBuf[1];
	lcd_spi.myTS_Calibrate.Bias_X = rdBuf[2];
	lcd_spi.myTS_Calibrate.Bias_Y = rdBuf[3];
	mrac_gain = isnanf(rdBuf[4]) != 0 ? 0.0001 : rdBuf[4];
	mrac_saturation = isnanf(rdBuf[5]) != 0 ? 1 : rdBuf[5];
	valve_delay = isnanf(rdBuf[6]) != 0 ? 1 : rdBuf[6];

	// Calibrate the touch if blue Nucleo board button is pressed
	if (HAL_GPIO_ReadPin(CALIBRATE_GPIO_Port, CALIBRATE_Pin) == GPIO_PIN_RESET) {
		TSC2046_Calibrate(&lcd_spi);
	}

	// Create initial frame
	bool update_main_frame = true;
	show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);

	// This will init CAN
	j1939.SA_ECU = 0x22;
	read_1939_message(&j1939);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LCDTask */
  LCDTaskHandle = osThreadNew(StartLCDTask, NULL, &LCDTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartControlTask, NULL, &ControlTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_10B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

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
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000;
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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim5, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

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
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
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
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TS_CS_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MCP2515_CS_GPIO_Port, MCP2515_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CALIBRATE_Pin */
  GPIO_InitStruct.Pin = CALIBRATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CALIBRATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_DC_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : TS_CS_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = TS_CS_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MCP2515_CS_Pin */
  GPIO_InitStruct.Pin = MCP2515_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MCP2515_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function should not be modified, when the callback is needed,
	 the HAL_UART_RxCpltCallback could be implemented in the user file
	 */

	// When we get 0x23 = '#', then send back new_output_float and new_input_float
	if (UART_RX_DATA[0] == 0x23) {
		convert.float_value = new_input0_float;
		UART_TX_DATA[0] = convert.uint8_values[0];
		UART_TX_DATA[1] = convert.uint8_values[1];
		UART_TX_DATA[2] = convert.uint8_values[2];
		UART_TX_DATA[3] = convert.uint8_values[3];
		convert.float_value = new_input1_float;
		UART_TX_DATA[4] = convert.uint8_values[0];
		UART_TX_DATA[5] = convert.uint8_values[1];
		UART_TX_DATA[6] = convert.uint8_values[2];
		UART_TX_DATA[7] = convert.uint8_values[3];
		convert.float_value = new_output_float;
		UART_TX_DATA[8] = convert.uint8_values[0];
		UART_TX_DATA[9] = convert.uint8_values[1];
		UART_TX_DATA[10] = convert.uint8_values[2];
		UART_TX_DATA[11] = convert.uint8_values[3];
		convert.float_value = new_reference_float;
		UART_TX_DATA[12] = convert.uint8_values[0];
		UART_TX_DATA[13] = convert.uint8_values[1];
		UART_TX_DATA[14] = convert.uint8_values[2];
		UART_TX_DATA[15] = convert.uint8_values[3];
		HAL_UART_Transmit(&huart2, UART_TX_DATA, 15, 10);
	}
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartLCDTask */
/**
 * @brief  Function implementing the LCDTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLCDTask */
void StartLCDTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	uint8_t FRAME_ID = 0;
	uint8_t SCREEN_ID = 0;
	for (;;) {
		// Read the LCD touch
		TSC2046_GetTouchData(&lcd_spi);
		uint8_t isPressed = lcd_spi.myTsData.isPressed;
		uint16_t X = lcd_spi.myTsData.X;
		uint16_t Y = lcd_spi.myTsData.Y;
		if (isPressed == true) {
			// Check which button we are pressing on
			if (X >= 8 && X <= 37 && Y >= 8 && Y <= 37) {
				if(MRAC_FRAME == FRAME_ID){
					// MRAC gain
					bool decimalbutton_show = true;
					bool minusbutton_show = false;
					show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &mrac_gain, "Set MRAC gain");
					bool update_main_frame = true;
					show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);
					SCREEN_ID = PLOT;
				}else if(J1939_FRAME == FRAME_ID){
					// Show address
					show_j1939_CA_names_frame(&lcd_spi, j1939.CA, j1939.SA_ACU);
					SCREEN_ID = ADDRESS;
				}

			} else if (X >= 8 && X <= 37 && Y >= 55 && Y <= 94) {
				if(MRAC_FRAME == FRAME_ID){
					// Set MRAC saturation
					bool decimalbutton_show = false;
					bool minusbutton_show = false;
					show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &mrac_saturation, "Set MRAC saturation");
					bool update_main_frame = true;
					show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);
					SCREEN_ID = PLOT;
				}else if(J1939_FRAME == FRAME_ID){
					// Set configuration
					show_j1939_configurations_frame(&lcd_spi, j1939.EEPROM_values, j1939.DM15_Status);
					SCREEN_ID = CONFIGURATION;
				}

			} else if (X >= 8 && X <= 37 && Y >= 100 && Y <= 139) {
				if(MRAC_FRAME == FRAME_ID){
					// Set valve delay
					bool decimalbutton_show = false;
					bool minusbutton_show = false;
					show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &valve_delay, "Set valve delay");
					bool update_main_frame = true;
					show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);
					SCREEN_ID = PLOT;
				}else if(J1939_FRAME == FRAME_ID){
					// Read error DM1 messages
					show_j1939_DM1_error_messages_frame(&lcd_spi, j1939.DM1_Messages_Binary);
					SCREEN_ID = DM1_MESSAGES;
				}
			} else if (X >= 8 && X <= 37 && Y >= 146 && Y <= 185) {
				if(MRAC_FRAME == FRAME_ID){
					// Open loop or closed loop
					if (closedloop_on == true)
						closedloop_on = false;
					else
						closedloop_on = true;
					bool update_main_frame = false;
					osDelay(300); // Prevent double click
					show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);
					SCREEN_ID = PLOT;
				}else if(J1939_FRAME == FRAME_ID){
					// Question dialog if you want to save the user settings
					uint8_t choice = show_question_save_settings_dialog(&lcd_spi); // Return 1 as YES and 0 as NO
					if (choice == 1) {
						// Save the touch calibration and the user settings
						float wrBuf[7] = { lcd_spi.myTS_Calibrate.Scale_X, lcd_spi.myTS_Calibrate.Scale_Y, lcd_spi.myTS_Calibrate.Bias_X, lcd_spi.myTS_Calibrate.Bias_Y, mrac_gain, mrac_saturation, valve_delay };
						FlashEraseSector(); // Must clear first
						FlashWriteN(0, wrBuf, 7, DATA_TYPE_FLOAT);
					}

					// Restore to normal again
					bool update_main_frame = true;
					show_j1939_main_frame(&lcd_spi, update_main_frame);
					// Same screen ID as before
				}
			} else if (X >= 8 && X <= 37 && Y >= 193 && Y <= 232) {
				// Change screen
				bool update_main_frame = true;
				if(MRAC_FRAME == FRAME_ID){
					FRAME_ID = J1939_FRAME;
					SCREEN_ID = ADDRESS;
					show_j1939_main_frame(&lcd_spi, update_main_frame);
				}else if(J1939_FRAME == FRAME_ID){
					FRAME_ID = MRAC_FRAME;
					SCREEN_ID = PLOT;
					show_main_frame_mrac(&lcd_spi, closedloop_on, update_main_frame);
				}
			} else if(X >= 100 && X <= 160 && Y >= 170 && Y <= 197){
				// We pressed a button - Change button or Reset for the J1939_FRAME
				if(J1939_FRAME == FRAME_ID){
					if(ADDRESS == SCREEN_ID){
						// Change button pressed
						rename_CA_SA = show_j1939_set_CA_names_numpad(&lcd_spi, j1939.CA, &j1939.SA_ACU); // Set the CA names and source address
						bool update_main_frame = true;
						show_j1939_main_frame(&lcd_spi, update_main_frame);
						show_j1939_CA_names_frame(&lcd_spi, j1939.CA, j1939.SA_ACU);
					}else if(DM1_MESSAGES == SCREEN_ID){
						// Reset button pressed
						reset_DM1_messages = true; // This will cause so we delete j1939.DM1_Messages_Binary array directly in other thread below
						show_j1939_DM1_error_messages_frame(&lcd_spi, j1939.DM1_Messages_Binary);
					}else if(CONFIGURATION == SCREEN_ID){
						// Change button pressed
						send_a_configuration_request_write = show_j1939_set_configurations_numpad(&lcd_spi, j1939.EEPROM_values, j1939.DM15_Status);
						bool update_main_frame = true;
						show_j1939_main_frame(&lcd_spi, update_main_frame);
						show_j1939_configurations_frame(&lcd_spi, j1939.EEPROM_values, j1939.DM15_Status);
					}
				}
			} else if(X >= 215 && X <= 277 && Y >= 170 && Y <= 197){
				// We pressed a button - Request button for the J1939_FRAME
				if(J1939_FRAME == FRAME_ID){
					if(ADDRESS == SCREEN_ID){
						// Pressed the request button
						send_a_claim_address_request = true; // Claim for a address by letting the other thread do the request first
						bool update_main_frame = true;
						show_j1939_main_frame(&lcd_spi, update_main_frame);
						show_j1939_CA_names_frame(&lcd_spi, j1939.CA, j1939.SA_ACU);
					}else if(CONFIGURATION == SCREEN_ID){
						// Pressed the request button
						send_a_configuration_request_read = true; // Request for a configurations by letting the other thread do the request first
						bool update_main_frame = true;
						show_j1939_main_frame(&lcd_spi, update_main_frame);
						show_j1939_configurations_frame(&lcd_spi, j1939.EEPROM_values, j1939.DM15_Status);
					}
				}
			}
			/* Debug coordinates
			char coordinates[40];
			sprintf(coordinates, "X=%d, Y=%d", X, Y);
			ILI9341_printText(&lcd_spi, coordinates, 100, 50, COLOR_RED, COLOR_NAVY, 1);
			*/
		} else {
			if(MRAC_FRAME == FRAME_ID){
				if(PLOT == SCREEN_ID){
					// Maximum plot can show is value 203
					uint16_t new_output_uint = map(new_output_float, -250, 250, 0, 203);
					uint16_t new_input0_uint = map(new_input0_float, -250, 250, 0, 203);
					uint16_t new_input1_uint = map(new_input1_float, -250, 250, 0, 203);
					uint16_t new_reference_uint = map(new_reference_float, -250, 250, 0, 203);
					bool only_move_array = false;
					bool clear_plot = false;
					show_plot_frame(&lcd_spi, new_input0_float, new_input1_float, new_output_float, new_reference_float, new_input0_uint, new_input1_uint, new_output_uint, new_reference_uint, only_move_array, clear_plot);
				}
			}else if(J1939_FRAME == FRAME_ID){
				// Add logic here
			}

		}

		osDelay(1);
	}
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_StartControlTask */
/**
 * @brief Function implementing the ControlTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument)
{
  /* USER CODE BEGIN StartControlTask */
	/* Infinite loop */
	float I1[RDIM] = { 0 }; // Integral for MRAC
	float I2[RDIM] = { 0 }; // Integral for MRAC
	float u[RDIM] = { 0 };
	float y[RDIM] = { 0 };
	float r[RDIM + 1] = { 0, 0 }; // Special case for the manual mode
	float x = 0; // State vector
	for (;;) {

		// Read the inputs and outputs
		r[0] = saturation((adcData[0] - 511) / 2, -250, 250);
		r[1] = saturation((adcData[1] - 511) / 2, -250, 250);
		Encoder_count(&encoder);
		y[0] = Encoder_getSpeed(&encoder) / one_rotation_pulses;

		// Do control mode now or manual mode
		if (closedloop_on) {

			// Set the reference so we can plot it
			new_reference_float = r[0];

			// Compute MRAC with the input signal u with saturation limit
			mrac(mrac_saturation, mrac_gain, y, u, r, I1, I2, RDIM);
			u[0] = saturation(u[0], -250, 250);

			// Compute the delay with Euler simulation for first order state space model with sampling time 1 ms
			x = x + 0.001 * (-1 / valve_delay * x + 1 / valve_delay * u[0]);

			// Control forward or backward
			if (u[0] > 0) {
				r[0] = u[0];
				r[1] = x;
			} else if (u[0] < 0) {
				r[0] = x;
				r[1] = u[0];
			} else {
				r[0] = 0;
				r[1] = 0;
				x = 0;
			}
		}else{
			new_reference_float = 0; // Plot this
		}

		// Set to plot
		new_input0_float = r[0];
		new_input1_float = r[1];
		new_output_float = y[0];

		// Set the control command to CAN-bus motors
		write_1939_auxiliary_valve_command(0x30, &j1939, r[0]);
		write_1939_auxiliary_valve_command(0x31, &j1939, r[1]);

		// Check if we got some CAN message
		read_1939_message(&j1939);

		// If we pressed "Flash" button
		if(rename_CA_SA){
			write_1939_CA_names_and_source_address(&j1939);
			rename_CA_SA = false;
		}
		// If we pressed "Request" button
		if(send_a_claim_address_request){
			write_1939_request_for_claim_address(&j1939);
			send_a_claim_address_request = false;
		}
		// If we pressed "Reset" button
		if(reset_DM1_messages){
			memset(j1939.DM1_Messages_Binary, 0, 8);
			j1939.DM15_Status = DM15_Proceed;
			reset_DM1_messages = false;
		}
		// If we pressed "Request" button
		if(send_a_configuration_request_read){
			write_1939_configuration_request(&j1939, 1); // Read
			send_a_configuration_request_read = false;
		}
		// If we pressed "Flash" button
		if(send_a_configuration_request_write){
			write_1939_configuration_request(&j1939, 2); // Write
			send_a_configuration_request_write = false;
		}

		osDelay(1);
	}
  /* USER CODE END StartControlTask */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
