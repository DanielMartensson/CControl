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
#include "MAX31865/MAX31865.h"
#include "FlashMemory/ReadWrite.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NP 1
#define NZ NP
#define NZE NP
#define ADIM NP
#define ADIMi 2
#define YDIM NP
#define RDIM NP
#define SAMPLING_TIME 60
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Definitions for LCD */
osThreadId_t LCDHandle;
const osThreadAttr_t LCD_attributes = {
  .name = "LCD",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 500 * 4
};
/* Definitions for CONTROL */
osThreadId_t CONTROLHandle;
const osThreadAttr_t CONTROL_attributes = {
  .name = "CONTROL",
  .priority = (osPriority_t) osPriorityHigh1,
  .stack_size = 3200 * 4
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM3_Init(void);
void LCD_TASK(void *argument);
void CONTROL_TASK(void *argument);

/* USER CODE BEGIN PFP */
void create_state_space_model();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* For the LCD */
bool closedloop_on = false; // Don't start with control mode

/* For the settings */
float setpoint = 0.0;
float horizon = 10;
float new_output_float = 0;
float new_input_float = 0;
float time_constant = 1;
ILI9341_SPI lcd_spi;
MAX31865_SPI temperature_spi;
uint8_t UART_RX_DATA[1];
Convert convert; // We borrow the convert Union from folder FlashMemory -> ReadWrite.h file
uint8_t UART_TX_DATA[8];

/* SISO Model - No integral action */
float A[ADIM * ADIM]; // System matrix
float B[ADIM * 1]; // Input matrix
float C[1 * ADIM]; // Output matrix

/* SISO Model - With integral action */
float Ai[ADIMi * ADIMi]; // System matrix
float Bi[ADIMi * 1]; // Input matrix
float Ci[1 * ADIMi]; // Output matrix


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
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

	// Start DMA for UART
	HAL_UART_Receive_DMA(&huart2, UART_RX_DATA, 1);

	// Set the address and sector for F446RE CPU where we are going to store the settings. Look at chapter 3.3 at document RM0390 for STM32F446XX processors
	FlashSetSectorAddrs(7, 0x08060000);

	// Init LCD
	ILI9341_Init(&lcd_spi, &hspi2, LCD_CS_GPIO_Port, LCD_CS_Pin, LCD_DC_GPIO_Port, LCD_DC_Pin, LCD_RST_GPIO_Port, LCD_RST_Pin, TS_CS_GPIO_Port, TS_CS_Pin);

	// Show welcome screen
	ILI9341_setRotation(&lcd_spi, 2);
	ILI9341_printImage(&lcd_spi, 0, 0, 320, 240, windows95_intro, 640 * 240 * sizeof(uint8_t));
	HAL_Delay(5000);

	// Load the user configurations
	float rdBuf[7];
	FlashReadN(0, rdBuf, 7, DATA_TYPE_FLOAT);
	lcd_spi.myTS_Calibrate.Scale_X = rdBuf[0];
	lcd_spi.myTS_Calibrate.Scale_Y = rdBuf[1];
	lcd_spi.myTS_Calibrate.Bias_X = rdBuf[2];
	lcd_spi.myTS_Calibrate.Bias_Y = rdBuf[3];
	setpoint = isnanf(rdBuf[4]) != 0 ? 0 : rdBuf[4];
	horizon = isnanf(rdBuf[5]) != 0 ? 10 : rdBuf[5];
	time_constant = isnanf(rdBuf[6]) != 0 ? 1 : rdBuf[6];

	// Create model
	create_state_space_model();

	// Calibrate the touch if blue Nucleo board button is pressed
	if (HAL_GPIO_ReadPin(CALIBRATE_GPIO_Port, CALIBRATE_Pin) == GPIO_PIN_RESET) {
		TSC2046_Calibrate(&lcd_spi);
	}

	// Create initial frame
	bool update_main_frame = true;
	show_main_frame(&lcd_spi, closedloop_on, update_main_frame);

	// Start PWM output for the SSR-relay
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

	// Init temperature PT100 reader
	MAX31865_init(&temperature_spi, MAX31865_CS_GPIO_Port, MAX31865_CS_Pin, &hspi3, WIRE4);
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
  /* creation of LCD */
  LCDHandle = osThreadNew(LCD_TASK, NULL, &LCD_attributes);

  /* creation of CONTROL */
  CONTROLHandle = osThreadNew(CONTROL_TASK, NULL, &CONTROL_attributes);

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
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
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 42000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000;
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
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_TIM_DISABLE_OCxPRELOAD(&htim3, TIM_CHANNEL_1);
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

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

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin|TS_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MAX31865_CS_GPIO_Port, MAX31865_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CALIBRATE_Pin */
  GPIO_InitStruct.Pin = CALIBRATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CALIBRATE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DC_Pin TS_CS_Pin */
  GPIO_InitStruct.Pin = LCD_DC_Pin|TS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX31865_CS_Pin */
  GPIO_InitStruct.Pin = MAX31865_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(MAX31865_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// When we get values from USB port, then read it directly
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function should not be modified, when the callback is needed,
	 the HAL_UART_RxCpltCallback could be implemented in the user file
	 */

	// When we get 0x23 = '#', then send back new_output_float and new_input_float
	if (UART_RX_DATA[0] == 0x23) {
		convert.float_value = new_input_float;
		UART_TX_DATA[0] = convert.uint8_values[0];
		UART_TX_DATA[1] = convert.uint8_values[1];
		UART_TX_DATA[2] = convert.uint8_values[2];
		UART_TX_DATA[3] = convert.uint8_values[3];
		convert.float_value = new_output_float;
		UART_TX_DATA[4] = convert.uint8_values[0];
		UART_TX_DATA[5] = convert.uint8_values[1];
		UART_TX_DATA[6] = convert.uint8_values[2];
		UART_TX_DATA[7] = convert.uint8_values[3];
		HAL_UART_Transmit(&huart2, UART_TX_DATA, 8, 10);
	}

}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void create_state_space_model(){
	// Create first order white box model
	A[0] = -1/time_constant;
	B[0] = 1/time_constant;
	C[0] = 1;
	// Turn it do discrete with sampling time SAMPLING_TIME seconds
	c2d(A, B, 1, 1, SAMPLING_TIME);
	// Create state space model with integral action
	Ai[0] = A[0];
	Ai[1] = B[0];
	Ai[2] = 0;
	Ai[3] = 1;
	Bi[0] = 0;
	Bi[1] = 1;
	Ci[0] = C[0];
	Ci[1] = 0;

}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_LCD_TASK */
/**
 * @brief  Function implementing the LCD thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_LCD_TASK */
void LCD_TASK(void *argument)
{
  /* USER CODE BEGIN 5 */
	/* Infinite loop */
	for (;;) {
		// Read the LCD touch
		TSC2046_GetTouchData(&lcd_spi);
		uint8_t isPressed = lcd_spi.myTsData.isPressed;
		uint16_t X = lcd_spi.myTsData.X;
		uint16_t Y = lcd_spi.myTsData.Y;
		if (isPressed == true) {
			// Check which button we are pressing on
			if (X >= 8 && X <= 37 && Y >= 8 && Y <= 37) {
				// Set point
				bool decimalbutton_show = true;
				bool minusbutton_show = false;
				show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &setpoint, "Set setpoint <= 100");
				if (setpoint > 100)
					setpoint = 100;
				bool update_main_frame = true;
				show_main_frame(&lcd_spi, closedloop_on, update_main_frame);
			} else if (X >= 8 && X <= 37 && Y >= 55 && Y <= 94) {
				// Set horizon
				bool decimalbutton_show = false;
				bool minusbutton_show = false;
				show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &horizon, "Set 0 < horizon <= 20");
				if(horizon == 0)
					horizon = 1;
				else if(horizon > 20)
					horizon = 20;
				bool update_main_frame = true;
				show_main_frame(&lcd_spi, closedloop_on, update_main_frame);
			} else if (X >= 8 && X <= 37 && Y >= 100 && Y <= 139) {
				// Set time constant
				bool decimalbutton_show = true;
				bool minusbutton_show = false;
				show_num_pad_frame(&lcd_spi, decimalbutton_show, minusbutton_show, &time_constant, "Set time constant >= 0.01");
				if(time_constant < 0.01)
					time_constant = 0.01;
				create_state_space_model();
				bool update_main_frame = true;
				show_main_frame(&lcd_spi, closedloop_on, update_main_frame);
			} else if (X >= 8 && X <= 37 && Y >= 146 && Y <= 185) {
				// Open loop or closed loop
				if (closedloop_on == true)
					closedloop_on = false;
				else
					closedloop_on = true;
				bool update_main_frame = false;
				osDelay(300); // Prevent double click
				show_main_frame(&lcd_spi, closedloop_on, update_main_frame);
			} else if (X >= 8 && X <= 37 && Y >= 193 && Y <= 232) {
				// Question dialog
				uint8_t choice = show_question_save_settings_dialog(&lcd_spi); // Return 1 as YES and 0 as NO
				if (choice == 1) {
					// Save the touch calibration and the user settings
					float wrBuf[7] = { lcd_spi.myTS_Calibrate.Scale_X, lcd_spi.myTS_Calibrate.Scale_Y, lcd_spi.myTS_Calibrate.Bias_X, lcd_spi.myTS_Calibrate.Bias_Y, setpoint, horizon, time_constant};
					FlashEraseSector(); // Must clear first
					FlashWriteN(0, wrBuf, 7, DATA_TYPE_FLOAT);
				}

				// Restore to normal again
				bool update_main_frame = true;
				show_main_frame(&lcd_spi, closedloop_on, update_main_frame);
			}
		} else {
			// Maximum plot can show is value 203
			uint16_t new_output_uint = map(new_output_float, 0, 100, 0, 203);
			uint16_t new_input_uint = map(new_input_float, 0, 100, 0, 203);
			bool only_move_array = false;
			bool clear_plot = false;
			show_plot_frame(&lcd_spi, new_input_float, new_output_float, new_input_uint, new_output_uint, only_move_array, clear_plot);
		}

		osDelay(1);
	}
  /* USER CODE END 5 */ 
}

/* USER CODE BEGIN Header_CONTROL_TASK */
/**
 * @brief Function implementing the CONTROL thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_CONTROL_TASK */
void CONTROL_TASK(void *argument)
{
  /* USER CODE BEGIN CONTROL_TASK */
	/* Infinite loop */

	/* Vectors */
	float xi[ADIMi] = { 0, 0 }; // Initial state vector with integral action
	float u[1] = { 0 }; // Input vector
	float y[1] = { 0 }; // Output vector
	float r[1] = { 0 }; // Reference vector
	float delta = 0;
	float past = 0;

	for (;;) {
		// Perform control if...
		if (closedloop_on == true) {
			// Reference
			r[0] = setpoint;

			// Linear programming with MPC - Here we are using saturation due to non-ideal model
			if(r[0] > y[0]){
				mpc(Ai, Bi, Ci, xi, u, r, ADIMi, YDIM, RDIM, horizon, 200, true); // 200 in iteration limit, true = integral action in the model
			}else{
				u[0] = 0;
			}

			// Integral state update for the model
			delta = u[0] - past;
			past = u[0];
			xi[0] = Ai[0]*xi[0] + Ai[1]*xi[1] + Bi[0]*delta + (y[0] - Ci[0]*xi[0]);
			xi[1] = Ai[2]*xi[0] + Ai[3]*xi[1] + Bi[1]*delta;

			// We will get very jumpy signals, prevent it
			if(delta < 0){
				u[0] = -delta + u[0]; // past
			}

			// Saturation and apply the inputs
			if(u[0] >= 100){
				htim3.Instance->CCR1 = 200.0 * 100; // 20000 = 100% PWM
				new_input_float = 100; // This is for the plot
			}else if(u[0] <= 0){
				htim3.Instance->CCR1 = 200.0 * 0; // 20000 = 100% PWM
				new_input_float = 0; // This is for the plot
			}else{
				htim3.Instance->CCR1 = 200.0 * u[0]; // 20000 = 100% PWM
				new_input_float = u[0]; // This is for the plot
			}


		} else {
			// Perform open loop control
			htim3.Instance->CCR1 = 200.0 * setpoint; // 20000 = 100% PWM
			new_input_float = setpoint; // This is for the plot
		}
		// Read temperature
		y[0] = MAX31865_temperature(&temperature_spi);
		new_output_float = y[0]; // This is for the plot

		osDelay(1);
	}

  /* USER CODE END CONTROL_TASK */
}

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
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
