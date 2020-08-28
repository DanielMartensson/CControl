/*
 * ReadWrite.c
 *
 *  Created on: Jul 5, 2020
 *      Author: Daniel Mårtensson
 */

#include "ReadWrite.h"

//Private variables
//1. sector start address
static uint32_t SectorAddrs;
static uint8_t SectorNum;

//functions definitions
//1. Erase Sector
void FlashEraseSector() {
	HAL_FLASH_Unlock();
	//Erase the required Flash sector
	FLASH_Erase_Sector(SectorNum, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
}

//2. Set Sector Adress
void FlashSetSectorAddrs(uint8_t sector, uint32_t addrs) {
	SectorNum = sector;
	SectorAddrs = addrs;
}

//3. Write Flash
void FlashWriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType) {
	uint32_t flashAddress = SectorAddrs + idx;

	// Converter
	Convert convert;
	uint8_t wrBuf2[4];

	//Unlock Flash
	HAL_FLASH_Unlock();
	//Write to Flash
	switch (dataType) {
	case DATA_TYPE_U8:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddress, ((uint8_t*) wrBuf)[i]);
			flashAddress++;
		}
		break;

	case DATA_TYPE_U16:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashAddress, ((uint16_t*) wrBuf)[i]);
			flashAddress += 2;
		}
		break;

	case DATA_TYPE_U32:
		for (uint32_t i = 0; i < Nsize; i++) {
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress, ((uint32_t*) wrBuf)[i]);
			flashAddress += 4;
		}
		break;

	case DATA_TYPE_FLOAT:
		for (uint32_t i = 0; i < Nsize; i++) {
			// Convert float -> uint8_t
			convert.float_value = ((float*) wrBuf)[i];
			wrBuf2[0] = convert.uint8_values[0];
			wrBuf2[1] = convert.uint8_values[1];
			wrBuf2[2] = convert.uint8_values[2];
			wrBuf2[3] = convert.uint8_values[3];
			// Write the uint8_t bytes
			FlashWriteN(idx + 4*i, wrBuf2, 4, DATA_TYPE_U8);
		}
		break;
	}
	//Lock the Flash space
	HAL_FLASH_Lock();
}
//4. Read Flash
void FlashReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType) {
	uint32_t flashAddress = SectorAddrs + idx;

	// Converter
	Convert convert;
	uint8_t rdBuf2[4];

	switch (dataType) {
	case DATA_TYPE_U8:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint8_t*) rdBuf + i) = *(uint8_t*) flashAddress;
			flashAddress++;
		}
		break;

	case DATA_TYPE_U16:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint16_t*) rdBuf + i) = *(uint16_t*) flashAddress;
			flashAddress += 2;
		}
		break;

	case DATA_TYPE_U32:
		for (uint32_t i = 0; i < Nsize; i++) {
			*((uint32_t*) rdBuf + i) = *(uint32_t*) flashAddress;
			flashAddress += 4;
		}
		break;

	case DATA_TYPE_FLOAT:
		for (uint32_t i = 0; i < Nsize; i++) {
			// Read the uint8_t bytes
			FlashReadN(idx + 4*i, rdBuf2, 4, DATA_TYPE_U8);
			// Convert uint8_t -> float
			convert.uint8_values[0] = rdBuf2[0];
			convert.uint8_values[1] = rdBuf2[1];
			convert.uint8_values[2] = rdBuf2[2];
			convert.uint8_values[3] = rdBuf2[3];
			*((float*) rdBuf + i) = convert.float_value;
		}
		break;
	}
}
