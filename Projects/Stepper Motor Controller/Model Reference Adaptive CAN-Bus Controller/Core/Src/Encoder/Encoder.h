/*
 * Encoder.h
 *
 *  Created on: 23 juil. 2020
 *      Author: Daniel Mårtensson
 */

#ifndef SRC_ENCODER_ENCODER_H_
#define SRC_ENCODER_ENCODER_H_

#include "main.h"

typedef struct{
	uint8_t dir;
	uint16_t cnt1, cnt2, diff, speed, one_rotation_pulses;
	uint32_t tick;
	TIM_HandleTypeDef* htim;
}Encoder;

void Encoder_init(Encoder *encoder, TIM_HandleTypeDef *htim, uint16_t one_rotation_pulses);
void Encoder_count(Encoder *encoder);
float Encoder_getSpeed(Encoder *encoder);
uint16_t Encoder_getDifference(Encoder *encoder);

#endif /* SRC_ENCODER_ENCODER_H_ */
