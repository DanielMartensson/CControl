/*
 * theta2ss.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "systemidentification.h"

/*
 * Convert SISO transfer function parameters theta vector into a
 * state space model on Observable Canonical Form.
 * Vector theta can be found from rls function.
 *
 * It's recommended that NP = NZ = NZE = row_a
 * No integral action:
 * A [row_a*row_a]
 * B [row_a*1]
 * C [1*row_a]
 * K [row_a*1]
 * theta [NP+NZ+NZE]
 *
 * Width integral action:
 * A [(row_a+1)*(row_a+1)]
 * B [(row_a+1)*1]
 * C [1*(row_a+1)]
 * K [(row_a+1)*1]
 */
void theta2ss(float A[], float B[], float C[], float theta[], float K[], size_t row_a, size_t NP, size_t NZ, bool integral_action){
	/* Clear A, C */
	memset(A, 0, row_a*row_a*sizeof(float));

	/* Decleartion */
	size_t i;

	if(integral_action == true){
		/* Insert A = [A 0; CA 1] */
		for (i = 0; i < NP; i++) {
			A[i * row_a] = -theta[i]; /* Insert on column 0 only to almost last row*/
		}
		for (i = 0; i < row_a - 2; i++) {
			A[i * row_a + 1 + i] = 1.0f; /* Ones at the diagonal with 1 in shift to right to third last row */
		}
		A[row_a*row_a -1] = 1; /* This adds 1 at the bottom corner to right  [A 0; CA ->1<-] */
		for (i = 0; i < NP; i++) {
			A[NP * row_a + i] = A[i]; /* This is exactly the same as C*A because C = [1 0] and C*A is the first row of A */
		}
		/* Insert B = [B; CB]*/
		for (i = 0; i < NZ; i++) {
			B[i] = theta[NP + i]; /* Insert the B in [B; CB] */
		}
		B[row_a - 1] = B[0]; /* This is C*B as long C = [1 0] */

		/* Insert C = [0 1] */
		memset(C, 0, row_a*sizeof(float));
		C[row_a-1] = 1.0f; /* Notice that transfer functions is only SISO */

		/* Create the kalman gain matrix K */
		for (i = 0; i < row_a - 1; i++) {
			K[i] = theta[NP + NZ + i] - theta[i];
		}
		K[row_a - 1] = 0.0f; /* Last element of K */
	}else{
		/* Insert A = [A] */
		for (i = 0; i < row_a; i++) {
			A[i * row_a] = -theta[i]; /* Insert on column 0 only to last row */
		}
		for (i = 0; i < row_a - 1; i++) {
			A[i * row_a + 1 + i] = 1.0f; /* Ones at the diagonal with 1 in shift to right to last row */
		}
		/* Insert B = [B] */
		for (i = 0; i < row_a; i++) {
			B[i] = theta[NP + i]; /* Insert the B in [B] */
		}
		/* Insert C = [1 0] */
		memset(C, 0, row_a*sizeof(float));
		C[0] = 1.0f; /* Notice that transfer functions is only SISO */

		/* Create the kalman gain matrix K */
		for (i = 0; i < row_a; i++) {
			K[i] = theta[NP + NZ + i] - theta[i];
		}
	}
}
