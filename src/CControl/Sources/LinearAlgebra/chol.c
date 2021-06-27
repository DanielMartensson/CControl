/*
 * chol.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Create A = L*L^T
 * A need to be symmetric
 * A [m*n]
 * L [m*n]
 * n == m
 */
void chol(float A[], float L[], uint16_t row) {
	memset(L, 0, row*row*sizeof(float));
	for (uint16_t i = 0; i < row; i++)
		for (uint16_t j = 0; j < (i + 1); j++) {
			float s = 0;
			for (uint16_t k = 0; k < j; k++)
				s += L[row * i + k] * L[row * j + k];

			// We cannot divide with zero
			if (L[row * j + j] == 0) {
				L[row * j + j] = FLT_EPSILON; // Same as eps command in MATLAB
			}
			L[row * i + j] = (i == j) ? sqrtf(A[row * i + i] - s) : (1.0 / L[row * j + j] * (A[row * i + j] - s));
		}
}

