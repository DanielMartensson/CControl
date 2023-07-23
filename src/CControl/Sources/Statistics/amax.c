/*
 * amax.c
 *
 *  Created on: 23 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

 /*
  * Compute the max value of vector x
  * x[L] Vector with values
  * max_index is the maximum index of the maximum value
  * L = Length of vector x
  */
float amax(float x[], size_t* max_index, size_t length) {
	float max_value = x[0];
	size_t i;
	for (i = 1; i < length; i++) {
		if (x[i] > max_value) {
			max_value = x[i];
			*max_index = i;
		}
	}
	return max_value;
}