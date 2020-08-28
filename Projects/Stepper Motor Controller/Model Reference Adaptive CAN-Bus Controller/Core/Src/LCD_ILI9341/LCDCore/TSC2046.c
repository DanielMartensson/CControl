/*
 * TSC2046.c
 *
 *  Created on: Feb 8, 2020
 *      Author: Muhammad Yaqoob, rewritted by Daniel Mårtensson
 */

#include "LCD_ILI9341.h"

//Functions definitions
// Send TSC2046 Command and wait for a response
static uint16_t TSC2046_SendCommand(ILI9341_SPI *spi, uint8_t cmd) {
	uint8_t spiBuf[3] = { 0, 0, 0 };
	uint16_t return16 = 0;

	HAL_GPIO_WritePin(spi->CS_PORT_TOUCH, spi->CS_PIN_TOUCH, GPIO_PIN_RESET);
	spiBuf[0] = cmd;
	HAL_SPI_Transmit(spi->lcdHandle, spiBuf, 1, 10);
	//Wait for response (3 ms)
	//HAL_Delay(3);
	if (HAL_SPI_Receive(spi->lcdHandle, &spiBuf[1], 2, 10) == HAL_OK){
		return16 = (spiBuf[1] << 4) + (spiBuf[2] >> 4);
	}else{
		return16 = 0;
	}
	HAL_GPIO_WritePin(spi->CS_PORT_TOUCH, spi->CS_PIN_TOUCH, GPIO_PIN_SET);

	return return16;
}

// Get raw touch data
//i. get x-axis raw touch 12-bit value
static uint16_t TSC2046_getRaw_X(ILI9341_SPI *spi) {
	return TSC2046_SendCommand(spi, CMD_X_AXIS | spi->CMD_Default);
}
//ii. get y-axis raw touch 12-bit value
static uint16_t TSC2046_getRaw_Y(ILI9341_SPI *spi) {
	return TSC2046_SendCommand(spi, CMD_Y_AXIS | spi->CMD_Default);
}
//iii. get z-axis raw touch 12-bit value
static uint16_t TSC2046_getRaw_Z(ILI9341_SPI *spi) {
	return TSC2046_SendCommand(spi, CMD_Z_AXIS | spi->CMD_Default);
}

// Print calibration points
//i. Top-Left corner point
static void TSC2046_TL_point(ILI9341_SPI *spi) {
	ILI9341_fillCircle(spi, 1, 1, 3, COLOR_RED);
	ILI9341_printText(spi, "Press here", 20, 30, COLOR_RED, COLOR_RED, 1);
}
//ii. Bottom-Right corner point
static void TSC2046_BR_point(ILI9341_SPI *spi) {
	ILI9341_fillCircle(spi, spi->myTS_Calibrate.Width - 1, spi->myTS_Calibrate.Height - 1, 3, COLOR_RED);
	ILI9341_printText(spi, "Press here", spi->myTS_Calibrate.Width - 80, spi->myTS_Calibrate.Height - 40, COLOR_RED, COLOR_RED, 1);
}

// Get orientation (from LCD driver)
static uint8_t TSC2046_getOrientation(ILI9341_SPI *spi) {
	return ILI9341_getRotation(spi);
}

// Poll for touch status
static void TSC2046_GetRawTouch(ILI9341_SPI *spi) {

	//Assign raw touch based on orientation
	switch (spi->ScreenOrientation) {
	case 1:
		spi->localRawTouch.x_touch = 4095 - TSC2046_getRaw_X(spi);
		spi->localRawTouch.y_touch = TSC2046_getRaw_Y(spi);
		spi->myTS_Calibrate.Width = 230;
		spi->myTS_Calibrate.Height = 320;
		break;

	case 2:
		spi->localRawTouch.x_touch = 4095 - TSC2046_getRaw_Y(spi);
		spi->localRawTouch.y_touch = 4095 - TSC2046_getRaw_X(spi);
		spi->myTS_Calibrate.Width = 320;
		spi->myTS_Calibrate.Height = 240;
		break;

	case 3:
		spi->localRawTouch.x_touch = TSC2046_getRaw_X(spi);
		spi->localRawTouch.y_touch = 4095 - TSC2046_getRaw_Y(spi);
		spi->myTS_Calibrate.Width = 230;
		spi->myTS_Calibrate.Height = 320;
		break;

	case 4:
		spi->localRawTouch.x_touch = TSC2046_getRaw_Y(spi);
		spi->localRawTouch.y_touch = TSC2046_getRaw_X(spi);
		spi->myTS_Calibrate.Width = 320;
		spi->myTS_Calibrate.Height = 240;
		break;
	}
	// These are for the calibration
	spi->myRawTouchDef.x_touch = spi->localRawTouch.x_touch;
	spi->myRawTouchDef.y_touch = spi->localRawTouch.y_touch;
}

// Calibrate resistive touch panel
void TSC2046_Calibrate(ILI9341_SPI *spi) {
	// Set rotation
	ILI9341_setRotation(spi, 2);

	uint16_t watchVar1 = 0;
	//Get Top-Left corner calibration coordinate
	TSC2046_TL_point(spi);
	spi->myTS_Calibrate.TL_X = 0;
	spi->myTS_Calibrate.TL_Y = 0;
	spi->myTS_Calibrate.BR_X = 0;
	spi->myTS_Calibrate.BR_Y = 0;
	spi->myRawTouchDef.y_touch = 0;
	spi->myRawTouchDef.x_touch = 0;

	while (1) {
		watchVar1 = TSC2046_getRaw_Z(spi);
		if (watchVar1 > 50) {
			for (uint8_t i = 0; i < 10; i++) {

				TSC2046_GetRawTouch(spi);
				spi->myTS_Calibrate.TL_X += spi->myRawTouchDef.x_touch;
				spi->myTS_Calibrate.TL_Y += spi->myRawTouchDef.y_touch;
			}

			break;
		}
		HAL_Delay(10);
	}
	HAL_Delay(1000);
	//Get Bottom-Right corner calibration coordinate
	TSC2046_BR_point(spi);
	while (1) {
		watchVar1 = TSC2046_getRaw_Z(spi);
		if (watchVar1 > 50) {
			for (uint8_t i = 0; i < 10; i++) {
				TSC2046_GetRawTouch(spi);
				spi->myTS_Calibrate.BR_X += spi->myRawTouchDef.x_touch;
				spi->myTS_Calibrate.BR_Y += spi->myRawTouchDef.y_touch;
			}
			break;
		}
		HAL_Delay(10);
	}

	spi->myTS_Calibrate.TL_X *= 0.1;
	spi->myTS_Calibrate.TL_Y *= 0.1;

	spi->myTS_Calibrate.BR_X *= 0.1;
	spi->myTS_Calibrate.BR_Y *= 0.1;

	//1. Calculate X_Diff, Y_Diff
	spi->myTS_Calibrate.Scale_X = (spi->myTS_Calibrate.Width + 0.0f) / (spi->myTS_Calibrate.BR_X - spi->myTS_Calibrate.TL_X + 0.0f);
	spi->myTS_Calibrate.Scale_Y = (spi->myTS_Calibrate.Height + 0.0f) / (spi->myTS_Calibrate.BR_Y - spi->myTS_Calibrate.TL_Y + 0.0f);
	//2. Calculate Scalling ()
	spi->myTS_Calibrate.Bias_X = -1 - spi->myTS_Calibrate.Scale_X * spi->myTS_Calibrate.TL_X; // -1 is for bias. Change this so you have 100% accuracy at center of the LCD
	spi->myTS_Calibrate.Bias_Y = -1 - spi->myTS_Calibrate.Scale_Y * spi->myTS_Calibrate.TL_Y; // -1 is for bias. Change this so you have 100% accuracy at center of the LCD

}

// Get touch sccreen data
void TSC2046_GetTouchData(ILI9341_SPI *spi) {

	uint16_t temp16x = 0, temp16y = 0;
	//Is screen pressed
	if (TSC2046_getRaw_Z(spi) > 50) {
		spi->myTsData.isPressed = 1; // True
		//Read touch data
		for (uint8_t i = 0; i < 10; i++) {
			TSC2046_GetRawTouch(spi);
			temp16x += spi->localRawTouch.x_touch;
			temp16y += spi->localRawTouch.y_touch;
		}
		spi->localRawTouch.x_touch = temp16x * 0.1;
		spi->localRawTouch.y_touch = temp16y * 0.1;
	} else
		spi->myTsData.isPressed = 0; // false

	//X_Touch value
	spi->myTsData.X = spi->myTS_Calibrate.Scale_X * spi->localRawTouch.x_touch + spi->myTS_Calibrate.Bias_X;

	//Y_Touch value
	spi->myTsData.Y = spi->myTS_Calibrate.Scale_Y * spi->localRawTouch.y_touch + spi->myTS_Calibrate.Bias_Y;
}
