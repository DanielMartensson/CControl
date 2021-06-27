/*
 * tran.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Turn A into transponse A^T
 */
void tran(float A[], uint16_t row, uint16_t column) {

	float B[row*column];
	float* transpose;
	float* ptr_A = A;

	for (uint16_t i = 0; i < row; i++) {
		transpose = &B[i];
		for (uint16_t j = 0; j < column; j++) {
			*transpose = *ptr_A;
			ptr_A++;
			transpose += row;
		}
	}

	// Copy!
	memcpy(A, B, row*column*sizeof(float));
}

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
