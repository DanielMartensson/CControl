/*
 * MCP3008.h
 *
 *  Created on: 22 maj 2019
 *      Author: root
 */

#ifndef MCP3008_MCP3008_H_
#define MCP3008_MCP3008_H_

#include "main.h"
#define Timeout 10
#define Size 1

void MCP3008_Init(SPI_HandleTypeDef hspi_, GPIO_TypeDef* GPIOx_, uint16_t GPIO_Pin_);
uint16_t MCP3008_Read_Channel(uint8_t channel);


#endif /* MCP3008_MCP3008_H_ */
