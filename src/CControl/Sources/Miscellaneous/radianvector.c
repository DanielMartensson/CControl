/*
 * radianvector.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Generate a radian vector with size N
 */
float* radianvector(const size_t N) {
	/* Allocate memory */
	float* x = (float*)malloc(N * sizeof(float));
	
	/* Split the full circle into N */
	const float d = 2.0f * PI / ((float)N);

	/* Create the radian vector */
	size_t i;
	x[0] = 0.0f;
	for (i = 1; i < N; i++) {
		x[i] = ((float)i) * d;
	}

	/* Return radian vector */
	return x;
}