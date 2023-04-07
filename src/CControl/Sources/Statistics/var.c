/*
 * var.c
 *
 *  Created on: 1 april 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Compute Bessel's variance
  * x[L] Vector with values
  * L = Length of vector x
  */
float var(float x[], uint16_t length) {
	float mu = mean(x, length);
	float sigma = 0.0f;
	float p;
	uint16_t i;
	for (i = 0; i < length; i++) {
		p = x[i] - mu;
		sigma += p * p;
	}
	return sigma / ((float)(length - 1));
}