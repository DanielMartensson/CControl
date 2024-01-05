/*
 * insert.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * This will insert matrix A, size row x column inside matrix B, row x column.
 * The variables start_row_b and start_column_b describe which row and column we should to insert first element of A into B
 * Notice that start positions are indexed from zero
 */
void insert(const float A[], float B[], const size_t row_a, const size_t column_a, const size_t column_b, const size_t start_row_b, const size_t start_column_b){
	/* Initial */
	B += start_row_b*column_b + start_column_b; /* Now we are standing on position start_row_b x start_column_b of row_b x column_b */

	/* We start at start_row_b and end at row_a, then we have inserted all rows from A into B */
	size_t i;
	const size_t column_a_bytes = column_a * sizeof(float);
	for(i = 0; i < row_a; i++){
		memcpy(B, A, column_a_bytes); /* Insert the complete row from A into B */
		A += column_a;
		B += column_b;
	}
}
