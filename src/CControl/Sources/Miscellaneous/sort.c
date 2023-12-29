/*
 * sort.c
 *
 *  Created on: 31 okt. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void insertion_sort(float X[], size_t index[], const size_t n, const SORT_MODE sort_mode);

/*
 * Sort a matrix
 * X[m*n]
 * index[m*n]
 */
void sort(float X[], size_t index[], const size_t row, const size_t column, const SORT_MODE sort_mode) {
	/* Decleration */
	size_t i, j;

	/* If we are sorting on rows */
	if (sort_mode == SORT_MODE_ROW_DIRECTION_ASCEND || sort_mode == SORT_MODE_ROW_DIRECTION_DESCEND) {
		/* Row major */
		tran(X, row, column);

		/* Sort on columns */
		sort(X, index, column, row, sort_mode == SORT_MODE_ROW_DIRECTION_ASCEND ? SORT_MODE_COLUMN_DIRECTION_ASCEND : SORT_MODE_COLUMN_DIRECTION_DESCEND);
		
		/* Restore back - Transpose */
		tran(X, column, row);
		size_t* indexT = (size_t*)malloc(row * column * sizeof(size_t));
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				indexT[i * column + j] = index[j * row + i];
			}
		}

		/* Copy over */
		memcpy(index, indexT, column * row * sizeof(size_t));

		/* Free */
		free(indexT);

		return;
	}
	
	/* Initial values for index */
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			index[i * column + j] = j;
		}
	}
	
	for (i = 0; i < row; i++) {
		insertion_sort(X, index, column, sort_mode);
		X += column;
		index += column;
	}
}

static void insertion_sort(float X[], size_t index[], const size_t column, const SORT_MODE sort_mode) {
	size_t i, j;
	float key;
	switch (sort_mode) {
	case SORT_MODE_COLUMN_DIRECTION_ASCEND:
		for (i = 1; i < column; i++) {
			key = X[i];
			j = i;

			/* Ascend */
			while ((j > 0) && (key < X[j - 1])) {
				X[j] = X[j - 1];
				index[j] = index[j - 1];
				--j;
			}

			X[j] = key;
			index[j] = i;
		}
		break;
	case SORT_MODE_COLUMN_DIRECTION_DESCEND:
		for (i = 1; i < column; i++) {
			key = X[i];
			j = i;

			/* Descend */
			while ((j > 0) && (key > X[j - 1])) {
				X[j] = X[j - 1];
				index[j] = index[j - 1];
				--j;
			}

			X[j] = key;
			index[j] = i;
		}
		break;
	}
}