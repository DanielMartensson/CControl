/*
 * lup.c
 *
 *  Created on: 2 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Do LU-decomposition with partial pivoting
 * A [m*n]
 * LU [m*n]
 * P [n]
 * n == m
 * Returns 1 == Success
 * Returns 0 == Fail
 */
uint8_t lup(float A[], float LU[], uint8_t P[], uint16_t row) {

	// If not the same
	if (A != LU) memcpy(LU, A, row*row*sizeof(float));

	// Create the pivot vector
	for (uint16_t i = 0; i < row; ++i)
		P[i] = i;

	for (uint16_t i = 0; i < row - 1; ++i) {
		int ind_max, tmp_int;

		ind_max = i;
		for (uint16_t j = i + 1; j < row; ++j)
			if (fabsf(LU[row * P[j] + i]) > fabsf(LU[row * P[ind_max] + i]))
				ind_max = j;

		tmp_int = P[i];
		P[i] = P[ind_max];
		P[ind_max] = tmp_int;

		if (fabsf(LU[row * P[i] + i]) < FLT_EPSILON)
			return 0; // matrix is singular (up to tolerance)

		for (uint16_t j = i + 1; j < row; ++j) {
			LU[row * P[j] + i] = LU[row * P[j] + i] / LU[row * P[i] + i];

			for (uint16_t k = i + 1; k < row; ++k)
				LU[row * P[j] + k] = LU[row * P[j] + k] - LU[row * P[i] + k] * LU[row * P[j] + i];
		}
	}

	return 1; // Solved
}

