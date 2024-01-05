/*
 * rot2d.c
 *
 *  Created on: 17 december 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/* 
 * Rotate the vector x[] that have size 2
 */
void rot2d(float x[], const float radians) {
	const float x0 = x[0] * cosf(radians) - x[1] * sinf(radians);
	const float x1 = x[0] * sinf(radians) + x[1] * cosf(radians);
	x[0] = x0;
	x[1] = x1;
}