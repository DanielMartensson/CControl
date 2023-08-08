/*
 * sort.c
 *
 *  Created on: 31 okt. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void insertion_sort(float X[], size_t index[], size_t n, SORT_MODE sort_mode);

/*
 * Sort a matrix
 * X[m*n]
 * index[m*n]
 */
void sort(float X[], size_t index[], size_t row, size_t column, SORT_MODE sort_mode){
	/* Decleration */
	size_t i, j;

	/* Initial values for index */
	if (sort_mode == SORT_MODE_ROW_DIRECTION_ASCEND || sort_mode == SORT_MODE_ROW_DIRECTION_DESCEND) {
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				index[i * column + j] = i;
			}
		}
	}
	else {
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				index[i * column + j] = j;
			}
		}
	}
	
	/* Save address */
	float* X0 = X;
	size_t* index0 = index;

	/* Declare outsider array Y */
	float* Y = (float*)malloc(row * sizeof(float));
	size_t* I = (size_t*)malloc(row * sizeof(size_t));

	if (sort_mode == SORT_MODE_ROW_DIRECTION_ASCEND || sort_mode == SORT_MODE_ROW_DIRECTION_DESCEND) {
		for (j = 0; j < column; j++) {
			/* Reset */
			X0 = X; 
			index0 = index;
			for (i = 0; i < row; i++) {
				/* Y[i] = X[i*column + j]; */
				Y[i] = X0[j];
				I[i] = index0[j];
				X0 += column;
				index0 += column;
			}

			/* Sort Y */
			insertion_sort(Y, I, row, sort_mode);

			/* Place back */
			X0 = X; /* Reset */
			index0 = index;
			for (i = 0; i < row; i++) {
				/* X[i*column + j] = Y[i]; */
				X0[j] = Y[i];
				index0[j] = I[i];
				X0 += column;
				index0 += column;
			}
		}
	}
	if(sort_mode == SORT_MODE_COLUMN_DIRECTION_ASCEND || sort_mode == SORT_MODE_COLUMN_DIRECTION_DESCEND){
		for(i = 0; i < row; i++) {
			insertion_sort(X0, index, column, sort_mode);
			X0 += column;
			index += column;
		}
	}

	/* Free */
	free(Y);
	free(I);
}


static void insertion_sort(float X[], size_t index[], size_t n, SORT_MODE sort_mode) {
	size_t i, j;
	for(i = 1; i < n; i++) {
		float key = X[i];
		j = i;
		if (sort_mode == SORT_MODE_COLUMN_DIRECTION_ASCEND || sort_mode == SORT_MODE_ROW_DIRECTION_ASCEND) {
			/* Ascend */
			while ((j > 0) && (key < X[j - 1])) {
				X[j] = X[j - 1];
				index[j] = index[j - 1];
				--j;
			}
		}
		if (sort_mode == SORT_MODE_COLUMN_DIRECTION_DESCEND || sort_mode == SORT_MODE_ROW_DIRECTION_DESCEND) {
			/* Descend */
			while( (j > 0) && (key > X[j - 1]) ) {
				X[j] = X[j - 1];
				index[j] = index[j - 1];
				--j;
			}
		}
		X[j] = key;
		index[j] = i;
	}
}