/*
 * mean.c
 *
 *  Created on: 26 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Compute the mean of vector x
 * x[L] Vector with values
 * L = Length of vector x
 */
float mean(float x[], uint16_t length) {
	float s = 0.0f;
	uint16_t i;
	for(i = 0; i < length; i++)
		s += x[i];
	return s/((float) length);
}
