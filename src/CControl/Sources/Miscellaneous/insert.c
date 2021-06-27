/*
 * insert.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * This will insert matrix A, size row x column inside matrix B, row x column.
 * The variables startRow_b and startColumn_b describe which row and column we should to insert first element of A into B
 * Notice that start positions are indexed from zero
 */
void insert(float A[], float B[], uint16_t row_a, uint16_t column_a, uint16_t column_b, uint16_t startRow_b, uint16_t startColumn_b){
	// Initial
	B += startRow_b*column_b + startColumn_b; // Now we are standing on position startRow_b x startColumn_b of row_b x column_b

	// We start at startRow_b and end at row_a, then we have inserted all rows from A into B
	for(uint16_t i = 0; i < row_a; i++){
		memcpy(B, A, column_a*sizeof(float)); // Insert the complete row from A into B
		A += column_a;
		B += column_b;
	}
}
