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
	float wr[ADIM]; // Real eigenvalues
	float wi[ADIM]; // Imaginary eigenvalues
	eig(A, wr, wi, ADIM);
	bool stable = true; // Assume that the system is stable
	for(uint8_t i = 0; i < ADIM; i++){
		float abs_value = sqrtf(wr[i]*wr[i] + wi[i]*wi[i]);
		if(abs_value > 1){
			stable = false;
		}
	}
	return stable;
}
