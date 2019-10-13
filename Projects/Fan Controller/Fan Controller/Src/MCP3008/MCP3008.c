/*
 * MCP3008.c
 *
 *  Created on: 22 maj 2019
 *      Author: Daniel Mårtensson
 */

#include "MCP3008.h"

/*
 * Parameter for MISO, MOSI, SCK, CS
 */
static GPIO_TypeDef* GPIOx;
static uint16_t GPIO_Pin; // CS
static SPI_HandleTypeDef hspi;

/*
 * Set the MISO, MOSI, SCK and CS
 */
void MCP3008_Init(SPI_HandleTypeDef hspi_, GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_){
	hspi = hspi_;
	GPIOx = GPIOx_; // Chip select
	GPIO_Pin = GPIO_Pin_;
}

/*
 * Read the channels from 0 to 7
 */
uint16_t MCP3008_Read_Channel(uint8_t channel){

	/*
	 * Declare data that we will send
	 */
	uint8_t pTxData[3] = {0};
	pTxData[0] = ((0x01 << 7)|				// start bit
				 (1 << 6)|					// SGL
				 ((channel & 0x07) << 3)); 	// channel number
	pTxData[1] = 0x00;
	pTxData[2] = 0x00;

	/*
	 * Data that we will get
	 */
	uint8_t pRxData[3] = {0};

	/*
	 * CS low, Send and receive, CS high
	 */
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi, &pTxData[0], &pRxData[0], Size, Timeout);
	HAL_SPI_TransmitReceive(&hspi, &pTxData[1], &pRxData[1], Size, Timeout);
	HAL_SPI_TransmitReceive(&hspi, &pTxData[2], &pRxData[2], Size, Timeout);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);

	/*
	 * Compute the ADC
	 */
	return 0x3FF & ((pRxData[0] & 0x01) << 9 | (pRxData[1] & 0xFF) << 1 | (pRxData[2] & 0x80) >> 7);
}
