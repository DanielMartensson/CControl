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
 * l = 1 = L1-norm
 * l = 2 = L2-norm
 */
float norm(float A[], uint16_t row, uint16_t column, uint8_t l){
	if(l == 1){
		// Vector
		if(row == 1){
			float sum_sqrt = 0;
			for(uint16_t i = 0; i < column; i++)
				sum_sqrt += sqrtf(A[i]*A[i]);
			return sum_sqrt;
		}else{
			// Matrix
			// MATLAB: sum(A, 1)
			for (uint16_t i = 1; i < row; i++) {
				for (uint16_t j = 0; j < column; j++) {
					A[j] += fabsf(A[i * column + j]);
				}
			}

			// Find the largest value on row 0
			float maxValue = A[0];
			for (uint16_t j = 1; j < column; j++) {
				if(A[j] > maxValue){
					maxValue = A[j];
				}
			}
			return maxValue;
		}
	}
	else if(l == 2){
		// Vector
		if(row == 1){
			float sqrt_sum = 0;
			for(uint16_t i = 0; i < column; i++)
				sqrt_sum += A[i]*A[i];
			return sqrtf(sqrt_sum);
		}else{
			// Matrix
			float U[row*column];
			float S[column];
			float V[column*column];
			if(row == column)
				svd_jacobi_one_sided(A, row, MAX_ITERATION_COUNT_SVD, U, S, V);
			else
				svd_golub_reinsch(A, row, column, U, S, V);
			float max_singular_value = 0;
			for(uint16_t i = 0; i < column; i++)
				if(S[i] > max_singular_value)
					max_singular_value = S[i];
			return max_singular_value;
		}
	}
	return 0;
	/* add more norms here */
}
