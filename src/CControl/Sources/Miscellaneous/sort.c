/*
 * sort.c
 *
 *  Created on: 31 okt. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void insertion_sort(float X[], uint16_t n, uint8_t mode);

/*
 * Sort a matrix
 * X[m*n]
 * dim = 1 = Row direction
 * dim = 2 = Column direction
 * mode = 1 = Ascend
 * mode = 2 = Descend
 */

void sort(float X[], uint16_t row, uint16_t column, uint8_t dim, uint8_t mode){
	/* Decleration */
	uint16_t i, j;
	
	/* Save address */
	float *X0 = X;

	/* Declare outsider array Y */
	float *Y = (float*)malloc(row * sizeof(float));

	switch(dim){
	case 1:
		for(j = 0; j < column; j++){
			X0 = X; /* Reset */
			for(i = 0; i < row; i++){
				/* Y[i] = X[i*column + j]; */
				Y[i] = X0[j];
				X0 += column;
			}

			/* Sort Y */
			insertion_sort(Y, row, mode);

			/* Place back */
			X0 = X; /* Reset */
			for(i = 0; i < row; i++){
				/* X[i*column + j] = Y[i]; */
				X0[j] = Y[i];
				X0 += column;
			}
		}
		break;
	case 2:
		for(i = 0; i < row; i++) {
			insertion_sort(&X0[0], column, mode);
			X0 += column;
		}
		break;
	}

	/* Free */
	free(Y);
}


static void insertion_sort(float X[], uint16_t n, uint8_t mode) {
	uint16_t i, j;
	for(i = 1; i < n; ++i) {
		float key = X[i];
		j = i;
		switch(mode){
		case 1:
			/* Ascend */
			while( (j > 0) && (key < X[j - 1]) ) {
				X[j] = X[j - 1];
				--j;
			}
			break;
		case 2:
			/* Descend */
			while( (j > 0) && (key > X[j - 1]) ) {
				X[j] = X[j - 1];
				--j;
			}
			break;
		}
		X[j] = key;
	}
}
