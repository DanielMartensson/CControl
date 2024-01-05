/*
 * cat.c
 *
 *  Created on: Nov 22, 2021
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Concatenate two matricies
 * A[ma*na]
 * B[mb*nb]
 * C[(ma + mb)*na] if row_wise = true
 * C[ma*(na + nb)] if row_wise = false
 */
void cat(const bool row_wise, const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const size_t column_b, const size_t row_c, const size_t column_c) {
	if (row_wise){
		/* C = [A; B] */
		memcpy(C, A, row_a * column_a * sizeof(float));
		memcpy(C + row_a * column_a, B, row_b * column_b * sizeof(float));
	}else{
		/* C = [A B] */
		size_t i;
		for (i = 0; i < row_a; i++) {
			memcpy(C, A, column_a * sizeof(float));
			C += column_a;
			A += column_a;
			memcpy(C, B, column_b * sizeof(float));
			C += column_b;
			B += column_b;
		}
	}
}
