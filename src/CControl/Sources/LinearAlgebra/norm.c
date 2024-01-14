/*
 * norm.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Find the norm of X. Use the NORM_METOD enum
 * A[m*n]
 */
float norm(const float X[], const size_t row, const size_t column, const NORM_METHOD norm_method){
	/* Create a new matrix A */
	float* A = (float*)malloc(row * column * sizeof(float));
	memcpy(A, X, row * column * sizeof(float));

	/* Decleration */
	size_t i, j;
	float return_value = 0;
	float* A0 = A;

	if(NORM_METHOD_L1 == norm_method){
		/* Vector */
		if((row == 1 && column > 0) || (row > 0 && column == 1)){
			j = row > column ? row : column;
			for (i = 0; i < j; i++) {
				return_value += fabsf(A[i]);
			}
		}else{
			/* Matrix */
			float max_value;
			for (j = 0; j < column; j++) {
				
				/* Reset */
				A = A0;

				/* Remember */
				max_value = return_value;
				return_value = 0;

				for (i = 0; i < row; i++) {
					return_value += fabsf(A[j]);

					/* Next row */
					A += column;
				}

				/* Check if return_value is larger than before */
				return_value = vmax(return_value, max_value);
			}
		}
	}
	else if(NORM_METHOD_L2 == norm_method){
		/* Vector */
		if((row == 1 && column > 0) || (row > 0 && column == 1)){
			float sqrt_sum = 0;
			j = row > column ? row : column;
			for (i = 0; i < j; i++) {
				sqrt_sum += A[i] * A[i];
			}
			return_value = sqrtf(sqrt_sum);
		}else{
			/* Matrix */
			float* U = (float*)malloc(row * column * sizeof(float));
			float* S = (float*)malloc(column * sizeof(float));
			float* V = (float*)malloc(column * column * sizeof(float));
			svd(A, row, column, U, S, V);

			/* Find maximum singular value */
			for (i = 0; i < column; i++) {
				if (S[i] > return_value) {
					return_value = S[i];
				}
			}

			/* Free */
			free(U);
			free(S);
			free(V);
		}
	}
	else if (NORM_METHOD_FROBENIUS == norm_method) {
		float sum_sqrt = 0;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				sum_sqrt += A[j] * A[j];
			}
			/* Next row */
			A += column;
		}
		return_value = sqrtf(sum_sqrt);
	}
	/* add more norms here */

	/* Reset */
	A = A0;

	/* Free */
	free(A);

	return return_value;
}
