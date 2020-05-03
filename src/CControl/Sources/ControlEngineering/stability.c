/*
 * stability.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This uses the dlyap function to check the stability
 * A need to be square. Returns 0 when system is not stable.
 * A is from the dx = Ax + Bu system
 * Returns 1 when system is stable.
 *
 * Works really good with models identified by RLS
 */
uint8_t stability(float* A, float* B, uint8_t ADIM, uint8_t RDIM){

	float P[ADIM*ADIM];

	// Create a symmetric matrix B*B' that is a positive definite matrix
	float Q[ADIM*ADIM];
	float Bt[ADIM];
	memcpy(Bt, B, ADIM*RDIM*sizeof(float));
	tran(Bt, ADIM, RDIM);
	mul(B, Bt, Q, ADIM, RDIM, ADIM);

	// Solve discrete lyapunov and find P
	dlyap(A, P, Q, ADIM);

	// Check if solution P is symmetric and positive definite
	for(int i = 0; i < ADIM; i++){
		for(int j = i; j < ADIM; j++){
			if(i == j){
				if(P[ADIM*i + j] <= 0) return 0; // Diagonals
			}else{
				float upper = P[ADIM*i + j];
				float lower = P[ADIM*j + i];
				if(upper <= 0 || lower <= 0 || upper - lower > FLT_EPSILON || upper - lower < -FLT_EPSILON) return 0; // non-Diagonals
			}
		}
	}

	return 1;
}


