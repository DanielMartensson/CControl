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
	size_t start, next, i;
	float tmp;
	for (start = 0; start <= column * row - 1; start++) {
		next = start;
		i = 0;
		do {
			i++;
			next = (next % row) * column + next / row;
		} while (next > start);
		if (next < start || i == 1) {
			continue;
		}
		tmp = A[next = start];
		do {
			i = (next % row) * column + next / row;
			A[next] = (i == start) ? tmp : A[i];
			next = i;
		} while (next > start);
	}
}

/* Old code
float* B = (float*)malloc(row * column * sizeof(float));
size_t i, j;
for (i = 0; i < row; i++) {
	for (j = 0; j < column; j++) {
		B[j * row + i] = A[i * column + j];
	}
}

// Copy over 
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
