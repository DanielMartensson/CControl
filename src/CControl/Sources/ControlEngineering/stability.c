/*
 * stability.c
 *
 *  Created on: 27 mai 2020
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"

/*
 * Check the stability of the matrix A by checking the eigenvalues
 * Return true if A is stable.
 * Return false is A is unstable
 */
bool stability(const float A[], const size_t row){
	size_t i = row * row * sizeof(float);
	float* dr = (float*)malloc(row * sizeof(float));		/* Real eigenvalues */
	float* di = (float*)malloc(row * sizeof(float));		/* Imaginary eigenvalues */
	float* wr = (float*)malloc(i);							/* Real eigenvectors */
	float* wi = (float*)malloc(i);							/* Imaginary eigenvectors */
	eig(A, dr, di, wr, wi, row);
	bool stable = true; /* Assume that the system is stable */
	float abs_value;
	for(i = 0; i < row; i++){
		abs_value = sqrtf(dr[i]*dr[i] + di[i]*di[i]);
		if(abs_value > 1){
			stable = false;
			break;
		}
	}
	free(dr);
	free(di);
	free(wr);
	free(wi);
	return stable;
}
