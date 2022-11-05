/*
 * sum.c
 *
 *  Created on: 3 nov. 2022
 *      Author: Daniel Mårtensson
 */


#include "../../Headers/Functions.h"

/*
 * Find the sum
 * x[m*n]
 * dim = 1 = Row direction
 * dim = 2 = Column direction
 * y[m] if dim = 1
 * y[n] if dim = 2
 */
void sum(float x[], float y[], uint16_t row, uint16_t column, uint8_t dim) {
	// Save address
	float *x0 = x;

	switch(dim){
	case 1:
		// Clear
		memset(y, 0, column*sizeof(float));

		// Fill
		for(uint16_t j = 0; j < column; j++){
			x0 = x; // Reset
			for(uint16_t i = 0; i < row; i++){
				y[j] += x0[j];
				x0 += column;
			}
		}
		break;
	case 2:
		// Clear
		memset(y, 0, row*sizeof(float));

		// Fill
		for(uint16_t i = 0; i < row; i++){
			for(uint16_t j = 0; j < column; j++){
				y[i] += x0[j];
			}
			x0 += column;
		}
		break;
	}
}
