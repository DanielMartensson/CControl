/*
 * tran.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Turn A into transponse A^T
 * A[m*n]
 */
void tran(float A[], const size_t row, const size_t column) {
	/* Declare */
	float* B = (float*)malloc(row * column * sizeof(float));
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			B[j * row + i] = A[i * column + j];
		}
	}

	/* Copy over */
	memcpy(A, B, row * column * sizeof(float));

	/* Free */
	free(B);
}

/* Old code
// Decleration
float* B = (float*)malloc(row * column * sizeof(float));
float* transpose = NULL;
float* ptr_A = A;
size_t i, j;

for (i = 0; i < row; i++) {
	transpose = &B[i];
	for (j = 0; j < column; j++) {
		*transpose = *ptr_A;
		ptr_A++;
		transpose += row;
	}
}

// Copy!
memcpy(A, B, row * column * sizeof(float));

// Free
free(B);
*/

/*
 * GNU Octave code:
 * >> A = [4 23 5; 2 45 5]
	A =

		4   23    5
		2   45    5

	>> A'
	ans =

		4    2
	   23   45
		5    5

	>>
 */
