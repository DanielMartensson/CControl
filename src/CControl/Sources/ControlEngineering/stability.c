/*
 * stability.c
 *
 *  Created on: 27 mai 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Check the stability of the matrix A by checking the eigenvalues
 * Return true if A is stable.
 * Return false is A is unstable
 */
bool stability(float A[], size_t ADIM){
	float *dr = (float*)malloc(ADIM * sizeof(float));			/* Real eigenvalues */
	float *di = (float*)malloc(ADIM * sizeof(float));			/* Imaginary eigenvalues */
	float* wr = (float*)malloc(ADIM * ADIM * sizeof(float));	/* Real eigenvectors */
	float* wi = (float*)malloc(ADIM * ADIM * sizeof(float));	/* Imaginary eigenvectors */
	eig(A, dr, di, di, wr, wi, ADIM);
	bool stable = true; /* Assume that the system is stable */
	size_t i;
	float abs_value;
	for(i = 0; i < ADIM; i++){
		abs_value = sqrtf(dr[i]*dr[i] + di[i]*di[i]);
		if(abs_value > 1){
			stable = false;
		}
	}
	free(dr);
	free(di);
	free(wr);
	free(wi);
	return stable;
}
