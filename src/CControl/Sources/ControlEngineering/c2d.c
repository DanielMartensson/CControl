/*
 * c2d.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

/*
 * Continuous to discrete
 * Turn A and B into discrete form
 * 
 * A[row_a*row_a]
 * B[row_a*column_b]
 */
void c2d(float A[], float B[], const size_t row_a, const size_t column_b, const float sampleTime) {
	size_t bytes_of_M = (row_a + column_b) * (row_a + column_b) * sizeof(float);
	float *M = (float*)malloc(bytes_of_M);
	memset(M, 0, bytes_of_M);
	size_t i, j;
	/* Create M = [A B; zeros(RDIM, ADIM) zeros(RDIM, RDIM)] */
	for (i = 0; i < row_a; i++) {
		/* For A row */
		for (j = 0; j < row_a; j++) {
			M[i * (row_a + column_b) + j] = A[i * row_a + j] * sampleTime;
		}
		/* For B row */
		for (j = 0; j < column_b; j++) {
			M[i * (row_a + column_b) + j + row_a] = B[i * column_b + j] * sampleTime;
		}
	}
	expm(M, row_a + column_b);
	for (i = 0; i < row_a; i++) {
		/* For A row */
		for (j = 0; j < row_a; j++) {
			A[i * row_a + j] = M[i * (row_a + column_b) + j];
		}
		/* For B row */
		for (j = 0; j < column_b; j++) {
			B[i * column_b + j] = M[i * (row_a + column_b) + j + row_a];
		}
	}

	/* Free */
	free(M);
}

/*
 * MATLAB:
 *   a1 = size(A,2) + size(B,2) - size(A,1);
	 b1 = size(A,2);
	 a2 = size(A,2) + size(B,2) - size(B,1);
	 b2 = size(B,2);
	 % Compute square matrix
	 M = [A B; zeros(a1, b1)  zeros(a2, b2)];
	 M = expm(M*h);
	 % Find the discrete matrecies
	 Ad = M(1:size(A,1), 1:size(A,2));
	 Bd = M(1:size(B,1), (size(A,2) + 1):(size(A,2) + size(B,2)));
 */

