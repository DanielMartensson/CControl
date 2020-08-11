/*
 * norm.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Find the norm of A
 * A[m*n]
 * l = 1 - 1-norm, the largest column sum of the absolute values of A.
 */
float norm(float A[], uint32_t row, uint32_t column, uint8_t l){
	if(l == 1){
		// MATLAB: sum(A, 1)
		for (uint32_t i = 1; i < row; i++) {
			for (uint32_t j = 0; j < column; j++) {
				A[j] += fabsf(A[i * column + j]);
			}
		}
		// Find the largest value on row 0
		float maxValue = A[0];
		for (uint32_t j = 1; j < column; j++) {
			if(A[j] > maxValue){
				maxValue = A[j];
			}
		}
		return maxValue;
	}
	return 0;
	/* add more norms here */
}

