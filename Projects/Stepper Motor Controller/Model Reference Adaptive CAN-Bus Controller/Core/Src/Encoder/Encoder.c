/*
 * Encoder.c
 *
 *  Created on: 23 juil. 2020
 *      Author: Daniel Mårtensson
 */

#include "Encoder.h"

/*
 * Use a timer and select combined channels -> Encoder Mode.
 * Then select our prescaler and the counter period
 * Then select Encoder Mode TI1 and TI2.
 * Leave the rest as default
 */
void Encoder_init(Encoder *encoder, TIM_HandleTypeDef *htim, uint16_t one_rotation_pulses) {
	encoder->htim = htim;
	encoder->one_rotation_pulses = one_rotation_pulses;
}

void Encoder_count(Encoder *encoder) {
	if (HAL_GetTick() - encoder->tick > 1000L) {
		/* Control TIM for every check_every_ms */
		encoder->cnt2 = encoder->htim->Instance->CNT;

		/* Control the rotation count */
		if (__HAL_TIM_IS_TIM_COUNTING_DOWN(encoder->htim)) {
			encoder->dir = 1;
			/* __HAL_TIM_IS_TIM_COUNTING_DOWN Exception when the macro is wrong configured */
			if ((encoder->cnt2 > encoder->cnt1) && (encoder->cnt2 - encoder->cnt1 < 100))
				encoder->dir = 0;
		} else {
			encoder->dir = 0;
			/* __HAL_TIM_IS_TIM_COUNTING_DOWN Excepting when the macro gives error */
			if ((encoder->cnt1 > encoder->cnt2) && (encoder->cnt1 - encoder->cnt2 < 100))
				encoder->dir = 1;
		}

		if (encoder->dir) {
			/* Down Counting */
			if (encoder->cnt1 >= encoder->cnt2)
				encoder->diff = encoder->cnt1 - encoder->cnt2;
			else
				encoder->diff = (encoder->htim->Instance->ARR + encoder->cnt1) - encoder->cnt2;
		} else {
			/* Up Counting */
			if (encoder->cnt2 >= encoder->cnt1)
				encoder->diff = encoder->cnt2 - encoder->cnt1;
			else
				encoder->diff = (encoder->htim->Instance->ARR + encoder->cnt2) - encoder->cnt1;
		}

		if ((encoder->htim->Instance->SMCR & 0x03) == 0x03) {
			/* X4 Mode Countering increase with 4 Pulse */
			encoder->speed = encoder->diff * 60 / 4 / encoder->one_rotation_pulses; //RPM x 60
		} else {
			/* X4 Mode Countering increase with 2 Pulse */
			encoder->speed = encoder->diff * 60 / 2 / encoder->one_rotation_pulses; //RPM x 60
		}

		encoder->tick = HAL_GetTick();
		encoder->cnt1 = encoder->htim->Instance->CNT;
	}
}

// Get the speed and also the direction - Unit: RPM*one_rotation_pulses
float Encoder_getSpeed(Encoder *encoder){
	return encoder->dir == 1 ? (float) encoder->speed : -((float) encoder->speed);
}

// Get the difference for every check
uint16_t Encoder_getDifference(Encoder *encoder){
	return encoder->diff;
}
