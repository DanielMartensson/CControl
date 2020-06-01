/*
 * theta2ss.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Convert SISO transfer function parameters theta vector into a
 * state space model on Observable Canonical Form.
 * Vector theta can be found from rls function.
 *
 * It's recommended that NP = NZ = NZE = ADIM
 * A [ADIM*ADIM]
 * B [ADIM*1]
 * C [1*ADIM]
 * K [ADIM*1]
 * theta [NP+NZ+NZE]
 */
void theta2ss(float* A, float* B, float* C, float* theta, float* K, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t NP, uint8_t NZ, uint8_t NZE){
	// Clear A, B, C, K
	memset(A, 0, ADIM*ADIM*sizeof(float)); // We need to set this to zeros to begin with
	//memset(B, 0, ADIM*RDIM*sizeof(float));
	//memset(C, 0, YDIM*ADIM*sizeof(float));
	//memset(K, 0, ADIM*YDIM*sizeof(float));


	// Insert A = [A]
	for(uint8_t i = 0; i < ADIM; i++){
		*(A + i*ADIM) = -*(theta + i); // Insert on column 0 only to last row
	}
	for(uint8_t i = 0; i < ADIM-1; i++){
		*(A + i*ADIM + 1 + i) = 1; // Ones at the diagonal with 1 in shift to right to last row
	}

	// Insert B = [B]
	for(uint8_t i = 0; i < ADIM; i++){
		*(B + i) = *(theta + NP + i); // Insert the B in [B]
	}

	// Insert C = [1 0]
	*(C + 0) = 1; // Notice that transfer functions is only SISO

	// Create the kalman gain matrix K
	for(uint8_t i = 0; i < ADIM; i++){
		*(K + i) = *(theta + NP + NZ + i) - *(theta + i);
	}
}
