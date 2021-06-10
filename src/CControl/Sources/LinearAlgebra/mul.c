/*
 * mul.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * C = A*B
 * A [row_a*column_a]
 * B [column_a*column_b]
 * C [row_a*column_b]
 */
void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b) {

	// Data matrix
	float* data_a;
	float* data_b;

	for (int i = 0; i < row_a; i++) {
		// Then we go through every column of b
		for (int j = 0; j < column_b; j++) {
			data_a = &A[i * column_a];
			data_b = &B[j];

			*C = 0; // Reset
			// And we multiply rows from a with columns of b
			for (int k = 0; k < column_a; k++) {
				*C += *data_a * *data_b;
				data_a++;
				data_b += column_b;
			}
			C++; // ;)
		}
	}
}

/*
 * GNU Octave code:
 *  >> A = [4 23; 2  5];
	>> B = [3; 1];
	>> C = A*B
	C =

	   35
	   11

	>>
 *
 */


