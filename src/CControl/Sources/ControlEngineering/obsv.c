/*
 * obsv.c
 *
 *  Created on: 28 June 2024
 *      Author: Daniel Mårtensson
 */


#include "controlengineering.h"

 /*
  * [C*A^1; C*A^2; C*A^3; ... ; C*A^N] % Extended observability matrix
  * A[row_a*row_a]
  * C[row_c*row_a]
  * Phi[(N*row_c)*row_a]
  */
void obsv(float Phi[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N) {
	/* Decleration */
	size_t i;

	/* This matrix will A^(i+1) all the time */
	float* A_copy = (float*)malloc(row_a * row_a * sizeof(float));
	memcpy(A_copy, A, row_a * row_a * sizeof(float));

	/* Temporary matrix */
	float* T = (float*)malloc(row_c * row_a * sizeof(float));

	/* Regular T = C*A^(1+i) */
	mul(C, A, T, row_c, row_a, row_a);

	/* Insert temporary T into Phi */
	memcpy(Phi, T, row_c * row_a * sizeof(float));

	/* Do the rest C*A^(i+1) because we have already done i = 0 */
	float* A_pow = (float*)malloc(row_a * row_a * sizeof(float));
	for (i = 1; i < N; i++) {
		mul(A, A_copy, A_pow, row_a, row_a, row_a); /*  Matrix power A_pow = A*A_copy */
		mul(C, A_pow, T, row_c, row_a, row_a); /* T = C*A^(1+i) */
		memcpy(Phi + i * row_c * row_a, T, row_c * row_a * sizeof(float)); /* Insert temporary T into PHI */
		memcpy(A_copy, A_pow, row_a * row_a * sizeof(float)); /* A_copy <- A_pow */
	}

	/* Free */
	free(A_copy);
	free(T);
	free(A_pow);
}

/*
GNU Octave code:

function Phi = phiMat(A, C, HORIZON)

  % Create the special Observabillity matrix
  Phi = [];
  for i = 1:HORIZON
	Phi = vertcat(Phi, C*A^i);
  end

end

*/
