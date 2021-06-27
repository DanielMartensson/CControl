/*
 * inv.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void solve(float A[], float x[], float b[], uint8_t P[], float LU[], uint16_t row);

/*
 * A to A^(-1)
 * Notice that only square matrices are only allowed.
 * Finding inverse should only be used for last solution.
 * Finding inverse is very cost expensive. Better to solve Ax=b instead
 * A[m*n]
 * n == m
 * Returns 1 == Success
 * Returns 0 == Fail
 */
uint8_t inv(float A[], uint16_t row) {

	// Create iA matrix
	float iA[row * row];

	// Create temporary matrix and variable
	float tmpvec[row];
	memset(tmpvec, 0, row * sizeof(float));
	uint8_t status = 0;

	// Check if the determinant is 0
	float LU[row * row];
	uint8_t P[row];
	status = lup(A, LU, P, row);
	if(status == 0)
		return 0; // matrix is singular. Determinant 0
	// Create the inverse
	for (uint16_t i = 0; i < row; ++i) {
		tmpvec[i] = 1.0;
		solve(A, &iA[row * i], tmpvec, P, LU, row);
		tmpvec[i] = 0.0;
	}

	// Transpose of iA
	tran(iA, row, row);

	// Copy over iA -> A
	memcpy(A, iA, row * row * sizeof(float));

	return status;
}

static void solve(float A[], float x[], float b[], uint8_t P[], float LU[], uint16_t row){
	// forward substitution with pivoting
	for (uint16_t i = 0; i < row; ++i) {
		x[i] = b[P[i]];

		for (uint16_t j = 0; j < i; ++j)
			x[i] = x[i] - LU[row * P[i] + j] * x[j];
	}

	// backward substitution with pivoting
	for (int16_t i = row - 1; i >= 0; --i) {
		for (int16_t j = i + 1; j < row; ++j)
			x[i] = x[i] - LU[row * P[i] + j] * x[j];

		x[i] = x[i] / LU[row * P[i] + i];
	}
}

/*
 * GNU Octave code:
 *   >> A = [3 4 5; 2 5 6; 5 6 7];
	 >> inv(A)
	 ans =

	 0.25000  -0.50000   0.25000
	 -4.00000   1.00000   2.00000
	 3.25000  -0.50000  -1.75000

	 >>
 */
