/*
 * sum.c
 *
 *  Created on: 3 nov. 2022
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/*
 * Find the sum
 * x[m*n]
 * y[n] if row_direction = true
 * y[m] if row_direction = false
 */
void sum(float x[], float y[], size_t row, size_t column, bool row_direction) {
	/* Decleration */
	size_t i, j;

	/* Save address */
	float* x0 = x;

	if (row_direction) {
		/* Clear */
		memset(y, 0, column * sizeof(float));

		/* Fill */
		for (j = 0; j < column; j++) {
			x0 = x; /* Reset */
			for (i = 0; i < row; i++) {
				y[j] += x0[j];
				x0 += column;
			}
		}
	}else{
		/* Clear */
		memset(y, 0, row * sizeof(float));

		/* Fill */
		for(i = 0; i < row; i++){
			for(j = 0; j < column; j++){
				y[i] += x0[j];
			}
			x0 += column;
		}
	}
}
