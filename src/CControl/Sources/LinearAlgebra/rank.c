/*
 * rank.c
 *
 *  Created on: 11 augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

size_t rank(float A[], size_t row, size_t column) {
	/* Compute the SVD */
	float* U = (float*)malloc(row * column * sizeof(float));
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	bool status = svd(A, row, column, U, S, V);

	/* Count the singular values in S that are over 0 */
	size_t i;
	size_t count = 0;
	for (i = 0; i < column; i++) {
		if (fabsf(S[i]) > MIN_VALUE){
			count++;
		}
	}
	
	/* Free */
	free(U);
	free(S);
	free(V);

	/* Return count */
	return status ? count : 0;
}