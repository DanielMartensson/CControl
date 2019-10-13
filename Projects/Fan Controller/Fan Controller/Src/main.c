/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MCP23017_LCD1602A/LCD16X2.h"
#include "MCP3008/MCP3008.h"
#include "CControl/Headers/Configurations.h"
#include "CControl/Headers/Functions.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MCP2307_ADDRESS 0x40
#define MCP2307_TRAILS 5
#define MCP2307_TIMEOUT 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  // Init the LCD
  LCD1602_mcp23017(hi2c1, MCP2307_ADDRESS, MCP2307_TRAILS, MCP2307_TIMEOUT, GPIOA, GPIO_PIN_5);
  LCD1602_Begin8BIT();
  LCD1602_noCursor();
  LCD1602_noBlink();

  LCD1602_print("LQI-Controller");
  LCD1602_2ndLine();

  // Init the MCP3008
  MCP3008_Init(hspi1, GPIOA, GPIO_PIN_10);

  // Init the PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  /* USER CODE END 2 */
  // Buffer for LCD
  char lcd_Text[16] = { 0 }; // Message

  // Calibrate temperature sensor MPC9700A
  float max_adc = 1023.0; // Maximum ADC value
  float V33 = 3.3; // MCP9700A voltage Vdd
  float m = 0.5; // Voltage when temperature is 0
  float k = (1.5 - 0.5) / (100.0 - 0.0); // Slope for MCP9700A

    // Model A, B, C, D
  float A[ADIM * ADIM] = { 0.99500201,  -0.00656006,  -0.00324621,   0.00284745,   0.00474606,   0.00299756,   0.00053557,   0.00510325,   0.00195878,   0.00103930,
    					   0.00656006,   0.99283815,  -0.02960128,   0.00526632,   0.01514675,   0.00524905,   0.01298864,   0.00695144,   0.01129487,   0.00493022,
    					  -0.00324621,   0.02960128,   0.99274160,   0.02925795,   0.03278408,   0.02712715,  -0.01623747,   0.03996273,   0.00092537,   0.00275946,
    					  -0.00284745,   0.00526632,  -0.02925795,   0.98010080,  -0.11692287,   0.00548336,  -0.13731362,   0.00838655,  -0.02404120,  -0.00952090,
    					   0.00474606,  -0.01514675,   0.03278408,   0.11692287,   0.13819930,  -0.75274525,   0.61524332,  -0.08864197,   0.03148846,   0.01481154,
    					  -0.00299756,   0.00524905,  -0.02712715,   0.00548336,   0.75274525,  -0.33361173,  -0.56018800,   0.01121465,  -0.02978725,  -0.00793863,
    					   0.00053557,  -0.01298864,  -0.01623747,   0.13731362,   0.61524332,   0.56018800,   0.49924238,  -0.16479061,  -0.02304370,  -0.00928901,
    					  -0.00510325,   0.00695144,  -0.03996273,   0.00838655,   0.08864197,   0.01121465,   0.16479061,   0.95896684,  -0.09604525,  -0.03548706,
    					   0.00195878,  -0.01129487,   0.00092537,   0.02404120,   0.03148846,   0.02978725,  -0.02304370,   0.09604525,   0.77327414,   0.61147580,
    					  -0.00103930,   0.00493022,  -0.00275946,  -0.00952090,  -0.01481154,  -0.00793863,   0.00928901,  -0.03548706,  -0.61147580,   0.78960433};
  float B[ADIM * RDIM] ={ -0.473079,
    					   0.246331,
    					  -0.174253,
    					  -0.121266,
    					   0.218141,
    					  -0.127719,
    					   0.051346,
    					  -0.214983,
    					   0.105775,
    					  -0.055904};
  float C[YDIM * ADIM] = {-0.0126102,  -0.0065661,  -0.0046448,   0.0032324,   0.0058147,   0.0034044,   0.0013687,   0.0057305,   0.0028195,   0.0014901};

  // Kalman steady state matrix K
  float K[ADIM * YDIM] = {-0.605326,
    					  -0.304048,
    					  -0.326261,
    					  -0.011849,
    					  -0.055601,
    					   0.171333,
    					   0.392499,
    					   0.167388,
    					   0.268947,
    					   0.014662};

  // Control law L
  float L[RDIM * ADIM] = {-0.748569,   0.336152,  -0.236959,  -0.072548,  -0.077530,  -0.558029,   0.771664,   0.406676,   0.483614,  -0.698279};

  // Integral law Li
  float Li[RDIM * 1] = {0.39778};

  // Initial states - Normally set to zero
  float x[ADIM * 1] = {0};
  float xi[RDIM * 1] = {0};

  //  Input signal, reference signal, output signal and state vector x and xi
  float qi = 0.5;		 // <-- Tune this in between 0->1, but qi =/= 1.
  float u[RDIM * 1] = {0}; // <-- This is our input value we want to have
  float r[RDIM * 1] = {25}; // <-- This is what we want to see.
  float y[YDIM * 1] = {0}; // <-- This is our measurement. Set it.

  /*
   * Anti-windup
   * 0 = Integrate xi when r < y and r > y
   * 1 = Delete xi when r < y, else integrate when r > y
   * 2 = Only integrate xi when r < y, but stop integrate when r > y
   */
  int anti_windup = 1;


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  // Read the potentiometer - 0 to 100
	  uint16_t adc0 = MCP3008_Read_Channel(1);
	  uint8_t reference = ((float) adc0) / 1023.0 * 100;

	  // Read the temperature sensor
	  uint16_t adc1 = MCP3008_Read_Channel(0);
	  float temperature = (((float) adc1) / max_adc * V33 - m) / k;
	  uint8_t temp_first = (uint8_t) temperature;
	  uint8_t temp_decimal = (uint8_t) ((temperature - ((float) temp_first)) * 100.0);

	  // Print out on LCD
	  sprintf(lcd_Text, "Ref:%d T:%d.%d  ", reference, temp_first, temp_decimal); // Important with 2 spaces at the end
	  LCD1602_setCursor(2, 1);
	  LCD1602_print(lcd_Text);

	  // Set reference from potentiometer and the temperature from MCP9700A
	  r[0] = (float) (reference);
	  y[0] = temperature;

	  // Compute the Linear Quadratic Integral control and reciuve the output u
	  lqi(y, u, qi, r, L, Li, x, xi, anti_windup);

	  // Estimate the state vector x
	  kalman(A, B, C, K, u, x, y);

	  // Do saturation where the input u[0] cannot be lower than 0 and greater than 100
	  saturation(&u[0], 0, 100);

	  // Change the speed of the fan
	  uint32_t output = (uint32_t) u[0]; //reference * 0.4; // 0.4 is a tuning parameter for the potentiometer
	  htim1.Instance->CCR1 = output;

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 40;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Green_LED_Board_Pin|Chip_Select_SPI1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Green_LED_Board_Pin Chip_Select_SPI1_Pin */
  GPIO_InitStruct.Pin = Green_LED_Board_Pin|Chip_Select_SPI1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
