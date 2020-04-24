/*
 * obsv.c
 *
 *  Created on: 1 mars 2020
 *      Author: hp
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * [C*A^1; C*A^2; C*A^3; ... ; C*A^HORIZON] % Extended observability matrix
 */
void obsv(float* PHI, float* A, float* C) {

	// This matrix will A^(i+1) all the time
	float A_pow[ADIM*ADIM];
	memset(A_pow, 0, ADIM * ADIM * sizeof(float));
	float A_copy[ADIM*ADIM];
	memcpy(A_copy, A, ADIM * ADIM * sizeof(float));

	// Temporary matrix
	float T[YDIM*ADIM];
	memset(T, 0, YDIM * ADIM * sizeof(float));

	// Regular T = C*A^(1+i)
	mul(C, A, T, YDIM, ADIM, ADIM);

	// Insert temporary T into PHI
	memcpy(PHI, T, YDIM*ADIM*sizeof(float));

	// Do the rest C*A^(i+1) because we have already done i = 0
	for(int i = 1; i < HORIZON; i++){
		mul(A, A_copy, A_pow, ADIM, ADIM, ADIM); //  Matrix power A_pow = A*A_copy
		mul(C, A_pow, T, YDIM, ADIM, ADIM); // T = C*A^(1+i)
		memcpy(PHI + i*YDIM*ADIM, T, YDIM*ADIM*sizeof(float)); // Insert temporary T into PHI
		memcpy(A_copy, A_pow, ADIM * ADIM * sizeof(float)); // A_copy <- A_pow
	}
}
