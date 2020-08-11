/*
 * c2d.c
 *
 *  Created on: 8 août 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Continuous to discrete
 * Turn A and B into discrete form
 */
void c2d(float A[], float B[], uint8_t ADIM, uint8_t RDIM, float sampleTime) {
	float M[(ADIM + RDIM) * (ADIM + RDIM)];
	memset(M, 0, sizeof(M));
	// Create M = [A B; zeros(RDIM, ADIM) zeros(RDIM, RDIM)]
	for (uint8_t i = 0; i < ADIM; i++) {
		// For A row
		for (uint8_t j = 0; j < ADIM; j++) {
			M[i * (ADIM + RDIM) + j] = A[i * ADIM + j] * sampleTime;
		}
		// For B row
		for (uint8_t j = 0; j < RDIM; j++) {
			M[i * (ADIM + RDIM) + j + ADIM] = B[i * RDIM + j] * sampleTime;
		}
	}
	expm(M, ADIM + RDIM);
	for (uint8_t i = 0; i < ADIM; i++) {
		// For A row
		for (uint8_t j = 0; j < ADIM; j++) {
			A[i * ADIM + j] = M[i * (ADIM + RDIM) + j];
		}
		// For B row
		for (uint8_t j = 0; j < RDIM; j++) {
			B[i * RDIM + j] = M[i * (ADIM + RDIM) + j + ADIM];
		}
	}
}

/*
 * MATLAB:
 *   a1 = size(A,2) + size(B,2) - size(A,1);
	 b1 = size(A,2);
	 a2 = size(A,2) + size(B,2) - size(B,1);
	 b2 = size(B,2);
	 % Compute square matrix
	 M = [A B; zeros(a1, b1)  zeros(a2, b2)];
	 M = expm(M*h);
	 % Find the discrete matrecies
	 Ad = M(1:size(A,1), 1:size(A,2));
	 Bd = M(1:size(B,1), (size(A,2) + 1):(size(A,2) + size(B,2)));
 */

