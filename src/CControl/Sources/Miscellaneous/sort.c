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
	// Declare outsider array Y
	float Y[row];

	switch(dim){
	case 1:
		for(uint16_t j = 0; j < column; j++){
			for(uint16_t i = 0; i < row; i++){
				Y[i] = X[i*column + j];
			}

			// Sort Y
			insertion_sort(Y, row, mode);

			// Place back
			for(uint16_t i = 0; i < row; i++){
				X[i*column + j] = Y[i];
			}
		}
		break;
	case 2:
		for(uint16_t i = 0; i < row; i++) {
			insertion_sort(&X[i*column], column, mode);
		}
		break;
	}
}


static void insertion_sort(float X[], uint16_t n, uint8_t mode) {
	for(uint16_t i = 1; i < n; ++i) {
		float key = X[i];
		uint16_t j = i;
		switch(mode){
		case 1:
			// Ascend
			while( (j > 0) && (key < X[j - 1]) ) {
				X[j] = X[j - 1];
				--j;
			}
			break;
		case 2:
			// Descend
			while( (j > 0) && (key > X[j - 1]) ) {
				X[j] = X[j - 1];
				--j;
			}
			break;
		}
		X[j] = key;
	}
}
