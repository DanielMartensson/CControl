/*
 * LCD16X2.h
 *
 *  Created on: 21 maj 2019
 *      Author: Daniel Mårtensson
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

#ifndef MCP23017_LCD1602A_LCD16X2_H_
#define MCP23017_LCD1602A_LCD16X2_H_



//***** List of COMMANDS *****//
#define LCD_CLEARDISPLAY 		0x01
#define LCD_RETURNHOME 			0x02
#define LCD_ENTRYMODESET 		0x04
#define LCD_DISPLAYCONTROL 		0x08
#define LCD_CURSORSHIFT 		0x10
#define LCD_FUNCTIONSET 		0x20
#define LCD_SETCGRAMADDR 		0x40
#define LCD_SETDDRAMADDR 		0x80

//***** List of commands Bitfields *****//
// Entry mode Bitfields
#define LCD_ENTRY_SH			0x01
#define LCD_ENTRY_ID			0x02
// Entry mode Bitfields
#define LCD_ENTRY_SH			0x01
#define LCD_ENTRY_ID			0x02
// Display control
#define LCD_DISPLAY_B			0x01
#define LCD_DISPLAY_C			0x02
#define LCD_DISPLAY_D			0x04
// Shift control
#define LCD_SHIFT_RL			0x04
#define LCD_SHIFT_SC			0x08
// Function set control
#define LCD_FUNCTION_F			0x04
#define LCD_FUNCTION_N			0x08
#define LCD_FUNCTION_DL			0x10

//Public functions
// ***** First run this connection *****//
void LCD1602_mcp23017(I2C_HandleTypeDef hi2c1_, uint16_t DevAddress_,uint32_t Trials_, uint32_t Timeout_, GPIO_TypeDef* GPIOx_,uint16_t GPIO_Pin_LED_);
// LCD begin 8 bits function
void LCD1602_Begin8BIT();
// LCD begin 4 bits function
void LCD1602_Begin4BIT();
// LCD print string
void LCD1602_print(char* string);
// set cursor position
void LCD1602_setCursor(uint8_t row, uint8_t col);
void LCD1602_1stLine();
void LCD1602_2ndLine();
// Enable two lines
void LCD1602_TwoLines();
void LCD1602_OneLine();
// Cursor ON/OFF
void LCD1602_noCursor();
void LCD1602_cursor();
// Clear display
void LCD1602_clear();
// Blinking cursor
void LCD1602_noBlink();
void LCD1602_blink();
// Display ON/OFF
void LCD1602_noDisplay();
void LCD1602_display();
//10) Shift Display, right or left
void LCD1602_shiftToRight(uint8_t num);
void LCD1602_shiftToLeft(uint8_t num);

//********** Print numbers to LCD **********//
// Integer
void LCD1602_PrintInt(int number);
// Float
void LCD1602_PrintFloat(float number, int decimalPoints);

#endif /* MCP23017_LCD1602A_LCD16X2_H_ */
