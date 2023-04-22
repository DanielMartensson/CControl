/*
 * norm.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Find the norm of X
 * A[m*n]
 * norm_method = 1 = L1 norm
 * norm_method = 2 = L2 norm
 * norm_method = 3 = Frobenius norm
 */
float norm(float X[], uint16_t row, uint16_t column, uint8_t norm_method){
	/* Create a new matrix A */
	float* A = (float*)malloc(row * column * sizeof(float));
	memcpy(A, X, row * column * sizeof(float));

	/* Decleration */
	uint16_t i, j;
	float return_value = 0;
	float* A0 = A;

	if(norm_method == 1){
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
	else if(norm_method == 2){
		/* Vector */
		if((row == 1 && column > 0) || (row > 0 && column == 1)){
			float sqrt_sum = 0, element;
			j = row > column ? row : column;
			for (i = 0; i < j; i++) {
				element = A[i];
				sqrt_sum += element * element;
			}
			return_value = sqrtf(sqrt_sum);
		}else{
			/* Matrix */
			float* U = (float*)malloc(row * column * sizeof(float));
			float* S = (float*)malloc(column * sizeof(float));
			float* V = (float*)malloc(column * column * sizeof(float));
			if (row == column) {
				svd_jacobi_one_sided(A, row, U, S, V);
			}
			else {
				svd_golub_reinsch(A, row, column, U, S, V);
			}

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
	else if (norm_method == 3) {
		float sum_sqrt = 0, element;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				element = A[j];
				sum_sqrt += element * element;
			}
			/* Next row */
			A += column;
		}
		return_value = sqrtf(sum_sqrt);
	}

	/* Reset */
	A = A0;

	/* Free */
	free(A);

	return return_value;
	/* add more norms here */
}
