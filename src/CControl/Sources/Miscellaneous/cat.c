/*
 * cat.c
 *
 *  Created on: Nov 22, 2021
 *      Author: ooaj
 */

#include <assert.h>
#include "../../Headers/functions.h"

/*
 * Concatenate two matricies
 * A[ma*na]
 * B[mb*nb]
 * C[(ma + mb)*na] if row_wise = true
 * C[ma*(na + nb)] if row_wise = false
 */
void cat(bool row_wise, float A[], float B[], float C[], size_t row_a, size_t column_a, size_t row_b, size_t column_b, size_t row_c, size_t column_c) {
	if (row_wise){
		/* C = [A; B] */
		assert(column_a == column_b);
		assert(row_a + row_b == row_c);
		memcpy(C, A, row_a * column_a * sizeof(float));
		memcpy(C + row_a * column_a, B, row_b * column_b * sizeof(float));
	}else{
		/* C = [A B] */
		assert(row_a == row_b);
		assert(column_a+ column_b == column_c);
		memcpy(C, A, column_a * sizeof(float));
		memcpy(C + column_a, B, column_b * sizeof(float));
		memcpy(C + column_a + column_b, A + column_a, column_a * sizeof(float));
		memcpy(C + column_a + column_b + column_a, B + column_b, column_b * sizeof(float));
	}
}
