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
 * No integral action:
 * A [ADIM*ADIM]
 * B [ADIM*1]
 * C [1*ADIM]
 * K [ADIM*1]
 * theta [NP+NZ+NZE]
 *
 * Width integral action:
 * A [(ADIM+1)*(ADIM+1)]
 * B [(ADIM+1)*1]
 * C [1*(ADIM+1)]
 * K [(ADIM+1)*1]
 */
void theta2ss(float A[], float B[], float C[], float theta[], float K[], uint8_t ADIM, uint8_t NP, uint8_t NZ, uint8_t NZE, bool integral_action){
	// Clear A, C
	memset(A, 0, ADIM*ADIM*sizeof(float));

	if(integral_action == true){
		// Insert A = [A 0; CA 1]
		for(uint8_t i = 0; i < NP; i++)
			A[i*ADIM] = -theta[i]; // Insert on column 0 only to almost last row
		for(uint8_t i = 0; i < ADIM-2; i++)
			A[i*ADIM + 1 + i] = 1; // Ones at the diagonal with 1 in shift to right to third last row
		A[ADIM*ADIM -1] = 1; // This adds 1 at the bottom corner to right  [A 0; CA ->1<-]
		for(uint8_t i = 0; i < NP; i++)
			A[NP*ADIM + i] = A[i]; // This is exactly the same as C*A because C = [1 0] and C*A is the first row of A

		// Insert B = [B; CB]
		for(uint8_t i = 0; i < NZ; i++)
			B[i] = theta[NP + i]; // Insert the B in [B; CB]
		B[ADIM-1] = B[0]; // This is C*B as long C = [1 0]

		// Insert C = [0 1]
		memset(C, 0, ADIM*sizeof(float));
		C[ADIM-1] = 1; // Notice that transfer functions is only SISO

		// Create the kalman gain matrix K
		for(uint8_t i = 0; i < ADIM-1; i++)
			K[i] = theta[NP + NZ + i] - theta[i];
		K[ADIM-1] = 0; // Last element of K
	}else{
		// Insert A = [A]
		for(uint8_t i = 0; i < ADIM; i++)
			A[i*ADIM] = -theta[i]; // Insert on column 0 only to last row
		for(uint8_t i = 0; i < ADIM-1; i++)
			A[i*ADIM + 1 + i] = 1; // Ones at the diagonal with 1 in shift to right to last row

		// Insert B = [B]
		for(uint8_t i = 0; i < ADIM; i++)
			B[i] = theta[NP + i]; // Insert the B in [B]

		// Insert C = [1 0]
		memset(C, 0, ADIM*sizeof(float));
		C[0] = 1; // Notice that transfer functions is only SISO

		// Create the kalman gain matrix K
		for(uint8_t i = 0; i < ADIM; i++)
			K[i] = theta[NP + NZ + i] - theta[i];
	}
}
