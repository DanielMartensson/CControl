/*
 * cut.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"


/*
 * Cut a matrix A with size row x columns into a matrix B with size (stop_row - start_row + 1) x (stop_column - start_column + 1).
 * Remember! Indexing start and stop are from zero!
 *
 * Example:
 * If you have a matrix A 5 x 6 and you want to cut the values from A to matrix B with size 3 x 3 and you want to start at 0,0 and end at 3,3
 * Code: cut(A, 5, 6, B, 0, 2, 0, 2); // Because indexing from zero
 */
void cut(float A[], uint16_t row, uint16_t column, float B[], uint16_t start_row, uint16_t stop_row, uint16_t start_column, uint16_t stop_column) {

	int in_columns = column;
	float* data = A + start_row * in_columns + start_column;

	// Create the output
	int out_columns = stop_column - start_column + 1;

	// Instead of having two for loops, we just copy the whole row at once.
	for (int i = start_row; i < stop_row + 1; i++) {
		memcpy(B, data, sizeof(float) * out_columns);
		B += out_columns;
		data += in_columns;
	}

}
