/*
 * stability.c
 *
 *  Created on: 27 mai 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Check the stability of the matrix A by checking the eigenvalues
 * Return true if A is stable.
 * Return false is A is unstable
 */
bool stability(float A[], uint8_t ADIM){
	float *wr = (float*)malloc(ADIM * sizeof(float)); /* Real eigenvalues */
	float *wi = (float*)malloc(ADIM * sizeof(float)); /* Imaginary eigenvalues */
	eig(A, wr, wi, ADIM);
	bool stable = true; /* Assume that the system is stable */
	uint8_t i;
	float abs_value;
	for(i = 0; i < ADIM; i++){
		abs_value = sqrtf(wr[i]*wr[i] + wi[i]*wi[i]);
		if(abs_value > 1){
			stable = false;
		}
	}
	free(wr);
	free(wi);
	return stable;
}
