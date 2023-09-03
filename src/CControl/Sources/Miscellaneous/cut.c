/*
 * cut.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Cut a matrix A with size row x columns into a matrix B with size (stop_row - start_row + 1) x (stop_column - start_column + 1).
 * Remember! Indexing start and stop are from zero!
 *
 * Example:
 * If you have a matrix A 5 x 6 and you want to cut the values from A to matrix B with size 3 x 3 and you want to start at 0,0 and end at 3,3
 * Code: cut(A, 5, B, 0, 2, 0, 2); // Because indexing from zero
 */
void cut(float A[], size_t column, float B[], size_t start_row, size_t stop_row, size_t start_column, size_t stop_column) {

	float *data = A + start_row * column + start_column;

	/* Create the output */
	size_t out_columns = stop_column - start_column + 1;
	size_t out_columns_bytes = out_columns * sizeof(float);

	/* Instead of having two for loops, we just copy the whole row at once. */
	size_t i;
	for (i = start_row; i < stop_row + 1; i++) {
		memcpy(B, data, out_columns_bytes);
		B += out_columns;
		data += column;
	}

}
