/*
 * LCD16X2.c
 *
 *  Created on: 21 maj 2019
 *      Author: Daniel Mårtensson
 */

#include "LCD16X2.h"

/*
 * Connect on MCP23017 to the LCD 16x2 for 8-bit mode
 * GPB0 - RS
 * GPB1 - RW
 * GPB2 - E
 *
 * GPA7 - D7
 * GPA6 - D6
 * GPA5 - D5
 * GPA4 - D4
 * GPA3 - D3
 * GPA2 - D2
 * GPA1 - D1
 * GPA0 - D0
 */

/*
 * Connect on MCP23017 to the LCD 16x2 for 4-bit mode
 * GPB0 - RS
 * GPB1 - RW
 * GPB2 - E
 *
 * GPA3 - D14
 * GPA2 - D13
 * GPA1 - D12
 * GPA0 - D11
 */

/*
 * Parameters for the I2C
 */
static uint8_t pData_A_side[2] = {0};
static uint8_t pData_B_side[2] = {0};
static const uint16_t Size = 2;
static I2C_HandleTypeDef hi2c1;
static uint16_t DevAddress;
static uint32_t Trials;
static uint32_t Timeout;
static GPIO_TypeDef* GPIOx;
static uint16_t GPIO_Pin_LED;
static uint8_t DisplayControl = 0x0F;
static uint8_t FunctionSet = 0x38;
static uint8_t mode_8_4_I2C = 1;
uint32_t writeTimeConstant = 10;

/*
 * Delay function
 */
static void LCD1602_TIM_MicorSecDelay(uint32_t uSecDelay) {
	TIM3->ARR = uSecDelay - 1;
	TIM3->SR &= ~(0x0001);  // Clear UEV flag
	TIM3->CR1 |= 1UL;
	while ((TIM3->SR & 0x0001) != 1);
}

/*
 * Enable EN pulse
 */
static void LCD1602_EnablePulse() {
	pData_B_side[0] = 0x13; // Calling B side
	pData_B_side[1] |= 0x04; // 1 0 0 = Only E will be enabled
	HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_B_side, Size, Timeout);
	LCD1602_TIM_MicorSecDelay(writeTimeConstant);
	pData_B_side[0] = 0x13; // Calling B side
	pData_B_side[1] &= ~0x04; // 1 0 0 -> 0 1 1 = Only E will be disabled
	HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_B_side, Size, Timeout);
	LCD1602_TIM_MicorSecDelay(60);
}

/*
 * RS control
 */
static void LCD1602_RS(bool state) {
	pData_B_side[0] = 0x13; // Calling B side
	if (state){
		pData_B_side[1] |= 0x01; // 0 0 1 = Only RS will be enabled
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_B_side, Size, Timeout);
	}else{
		pData_B_side[1] &= ~0x01; // 0 0 1 -> 1 1 0 = Only RS will be disabled
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_B_side, Size, Timeout);
	}
}

/*
 * Write Parallel interface
 */
static void LCD1602_write(uint8_t byte) {
	pData_A_side[0] = 0x12; // Calling A side
	if (mode_8_4_I2C == 1){ //8bits mode
		pData_A_side[1] = byte;
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_A_side, Size, Timeout);
		LCD1602_EnablePulse();
	} else if (mode_8_4_I2C == 2){	//4 bits mode
		pData_A_side[1] = (byte >> 4) & 0xF; // Last 4 bytes - Look at the datasheet
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_A_side, Size, Timeout);
		LCD1602_EnablePulse();
		pData_A_side[1] = byte & 0xF; // First 4 bytes
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_A_side, Size, Timeout);
		LCD1602_EnablePulse();
	}
}
/*
 *  Microsecond delay functions
 */
static void LCD1602_TIM_Config() {
	RCC_ClkInitTypeDef myCLKtypeDef;
	uint32_t clockSpeed;
	uint32_t flashLatencyVar;
	HAL_RCC_GetClockConfig(&myCLKtypeDef, &flashLatencyVar);
	if (myCLKtypeDef.APB1CLKDivider == RCC_HCLK_DIV1) {
		clockSpeed = HAL_RCC_GetPCLK1Freq();
	} else {
		clockSpeed = HAL_RCC_GetPCLK1Freq() * 2;
	}
	clockSpeed *= 0.000001;

	//Enable clock for TIM2 timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // 0x1
	//Set the mode to Count up
	TIM3->CR1 &= ~(0x0010);
	//Enable Update Event
	TIM3->CR1 &= ~(0x0001);
	//Update request source
	TIM3->CR1 &= ~(1UL << 2);
	// Set bit 3 High to enable One-Pulse mode
	TIM3->CR1 |= (1UL << 3);
	//Set the Prescalar
	TIM3->PSC = clockSpeed - 1;
	//Set and Auto-Reload Value to delay the timer 1 sec
	TIM3->ARR = 10 - 1; 						// The Flag sets when overflows
	//Event generation handling to reset the counter
	TIM3->EGR = 1; 					//Update generate auto
	TIM3->SR &= ~(0x0001);	//Clear Update interrupt flag
}

/*
 * Write command
 */
static void LCD1602_writeCommand(uint8_t command) {
	//Set RS to 0
	LCD1602_RS(false);
	//Call low level write parallel function
	LCD1602_write(command);
}
/*
 * Write 8 bits data
 */
static void LCD1602_writeData(uint8_t data) {
	//Set RS to 1
	LCD1602_RS(true);
	//Call low level write parallel function
	LCD1602_write(data);
}

/*
 * Write 4 bits command, *FOR 4 BITS MODE ONLY*
 */
static void LCD1602_write4bitCommand(uint8_t nibble) {
	pData_A_side[0] = 0x12; // Calling A side
	//Set RS to 0
	LCD1602_RS(false);
	pData_A_side[1] = nibble & 0xF;
	HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_A_side, Size, Timeout);
	LCD1602_EnablePulse();
}

/*
 * Initial for MCP23017 i2c connection to the LCD
 */
void LCD1602_mcp23017(I2C_HandleTypeDef hi2c1_, uint16_t DevAddress_,uint32_t Trials_, uint32_t Timeout_, GPIO_TypeDef* GPIOx_,uint16_t GPIO_Pin_LED_) {
	/*
	 * Set the parameters first
	 */
	hi2c1 = hi2c1_;
	DevAddress = DevAddress_;
	Trials = Trials_;
	Timeout = Timeout_;
	GPIOx = GPIOx_;
	GPIO_Pin_LED = GPIO_Pin_LED_;

	/*
	 * Check connection
	 */
	if (HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, Timeout) == HAL_OK) {
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin_LED); // Connection. Light up GPIOx at pin GPIO_Pin_LED
		/*
		 * Set output for A-side and B-side
		 */
		pData_A_side[0] = 0x00;
		pData_A_side[1] = 0x00;
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_A_side, Size, Timeout);

		/*
		 * B-side
		 */
		pData_B_side[0] = 0x01;
		HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData_B_side, Size, Timeout);

	}
}

/*
 * LCD begin 8 bits function
 */
void LCD1602_Begin8BIT() {
	//Initialise microsecond timer
	LCD1602_TIM_Config();
	//Set the mode to 8 bits
	mode_8_4_I2C = 1;
	//Function set variable to 8 bits mode
	FunctionSet = 0x38;
	//Initialise LCD
	//1. Wait at least 15ms
	HAL_Delay(20);
	//2. Attentions sequence
	LCD1602_writeCommand(0x30);
	HAL_Delay(5);
	LCD1602_writeCommand(0x30);
	HAL_Delay(1);
	LCD1602_writeCommand(0x30);
	HAL_Delay(1);
	//3. Function set; Enable 2 lines, Data length to 8 bits
	LCD1602_writeCommand(LCD_FUNCTIONSET | LCD_FUNCTION_N | LCD_FUNCTION_DL);
	//4. Display control (Display ON, Cursor ON, blink cursor)
	LCD1602_writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_B | LCD_DISPLAY_C | LCD_DISPLAY_D);
	//5. Clear LCD and return home
	LCD1602_writeCommand(LCD_CLEARDISPLAY);
	HAL_Delay(2);
}

/*
 * LCD begin 4 bits function
 */
void LCD1602_Begin4BIT() {
	//Initialise microsecond timer
	LCD1602_TIM_Config();
	//Set the mode to 4 bits
	mode_8_4_I2C = 2;
	//Function set variable to 4 bits mode
	FunctionSet = 0x28;
	//Initialise LCD
	//1. Wait at least 15ms
	HAL_Delay(20);
	//2. Attentions sequence
	LCD1602_write4bitCommand(0x3);
	HAL_Delay(5);
	LCD1602_write4bitCommand(0x3);
	HAL_Delay(1);
	LCD1602_write4bitCommand(0x3);
	HAL_Delay(1);
	LCD1602_write4bitCommand(0x2);  //4 bit mode
	HAL_Delay(1);
	//3. Display control (Display ON, Cursor ON, blink cursor)
	LCD1602_writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAY_B | LCD_DISPLAY_C | LCD_DISPLAY_D);
	//4. Clear LCD and return home
	LCD1602_writeCommand(LCD_CLEARDISPLAY);
	HAL_Delay(3);
	//4. Function set; Enable 2 lines, Data length to 8 bits
	LCD1602_writeCommand(LCD_FUNCTIONSET | LCD_FUNCTION_N);
	HAL_Delay(3);
}

/*
 * LCD print string
 */
void LCD1602_print(char* string) {
	for (uint8_t i = 0; i < 16 && string[i] != 0; i++) {
		LCD1602_writeData((uint8_t) string[i]);
	}
}

/*
 * set cursor position
 */
void LCD1602_setCursor(uint8_t row, uint8_t col) {
	uint8_t maskData;
	maskData = (col - 1) & 0x0F;
	if (row == 1) {
		maskData |= (0x80);
		LCD1602_writeCommand(maskData);
	} else {
		maskData |= (0xc0);
		LCD1602_writeCommand(maskData);
	}
}

/*
 * Write on first line
 */
void LCD1602_1stLine(void) {
	LCD1602_setCursor(1, 1);
}

/*
 * Write on second line
 */
void LCD1602_2ndLine(void) {
	LCD1602_setCursor(2, 1);
}

/*
 * Enable two lines
 */
void LCD1602_TwoLines(void) {
	FunctionSet |= (0x08);
	LCD1602_writeCommand(FunctionSet);
}

/*
 * Enable one line
 */
void LCD1602_OneLine(void) {
	FunctionSet &= ~(0x08);
	LCD1602_writeCommand(FunctionSet);
}

/*
 * Cursor OFF
 */
void LCD1602_noCursor(void) {
	DisplayControl &= ~(0x02);
	LCD1602_writeCommand(DisplayControl);
}

/*
 * Display Cursor
 */
void LCD1602_cursor(void) {
	DisplayControl |= (0x02);
	LCD1602_writeCommand(DisplayControl);
}

/*
 * Clear display
 */
void LCD1602_clear(void) {
	LCD1602_writeCommand(LCD_CLEARDISPLAY);
	HAL_Delay(3);
}

/*
 * No Blinking cursor
 */
void LCD1602_noBlink(void) {
	DisplayControl &= ~(0x01);
	LCD1602_writeCommand(DisplayControl);
}

/*
 * Blinking cursor
 */
void LCD1602_blink(void) {
	DisplayControl |= 0x01;
	LCD1602_writeCommand(DisplayControl);
}

/*
 * Display OFF
 */
void LCD1602_noDisplay(void) {
	DisplayControl &= ~(0x04);
	LCD1602_writeCommand(DisplayControl);
}

/*
 * LCD Display
 */
void LCD1602_display(void) {
	DisplayControl |= (0x04);
	LCD1602_writeCommand(DisplayControl);
}

/*
 * LCD shift to right
 */
void LCD1602_shiftToRight(uint8_t num) {
	for (uint8_t i = 0; i < num; i++) {
		LCD1602_writeCommand(0x1c);
	}
}

/*
 * LCD shift to left
 */
void LCD1602_shiftToLeft(uint8_t num) {
	for (uint8_t i = 0; i < num; i++) {
		LCD1602_writeCommand(0x18);
	}
}

/*
 * Print integer
 */
void LCD1602_PrintInt(int number) {
	char numStr[16];
	sprintf(numStr, "%d", number);
	LCD1602_print(numStr);
}

/*
 * Print float
 */
void LCD1602_PrintFloat(float number, int decimalPoints) {
	char numStr[16];
	sprintf(numStr, "%.*f", decimalPoints, number);
	LCD1602_print(numStr);
}
