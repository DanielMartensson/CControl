/*
 * shuffle.c
 *
 *  Created on: 28 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Shuffle data 
 * X[m*n]
 * index[m] if row_wise = true
 * index[n] if row_wise = false
 */
void shuffle(float X[], size_t index[], const bool row_wise, const size_t row, const size_t column) {
	/* Activate the srand */
	randn(NULL, 0, 0.0f, 0.0f);
	
	/* Shuffle the data */
	size_t i;
	if (row_wise) {
		const size_t column_bytes = column * sizeof(float);
		float* temp = (float*)malloc(column_bytes);
		for (i = 0; i < row; i++) {
			index[i] = i;
		}
		for (i = 0; i < row; i++) {
			size_t p = randi(row);
			/* Do swapping */
			SWAP(index[p], index[i], size_t);
			memcpy(temp, X + i * column, column_bytes);
			memcpy(X + i * column, X + p * column, column_bytes);
			memcpy(X + p * column, temp, column_bytes);
		}

		/* Free */
		free(temp);
	}
	else {
		size_t j;
		for (j = 0; j < column; j++) {
			index[j] = j;
		}
		for (j = 0; j < column; j++) {
			size_t p = randi(column);
			SWAP(index[p], index[j], size_t);
			for (i = 0; i < row; i++) {
				SWAP(X[i * column + p], X[i * column + j], float);
			}
		}
	}
}