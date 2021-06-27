/*
 * std.c
 *
 *  Created on: 26 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"


/*
 * Compute Standard deviation
 * x[L] Vector with values
 * L = Length of vector x
 */
float std(float x[], uint16_t length) {
	float mu = mean(x, length);
	float sigma = 0;
	for(uint16_t i = 0; i < length; i++)
		sigma += (x[i] - mu) * (x[i] - mu);
	return sqrtf(sigma/((float) length));
}
