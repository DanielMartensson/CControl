/*
 * kalman.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * This is linear Kalman filter state update
 * x = Ax - KCx + Bu + Ky
 */
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM) {

	// Compute the vector A_vec = A*x
	float A_vec[ADIM * 1];
	mul(A, x, A_vec, ADIM, ADIM, 1);

	// Compute the vector B_vec = B*u
	float B_vec[ADIM * 1];
	mul(B, u, B_vec, ADIM, RDIM, 1);

	// Compute the vector C_vec = C*x
	float C_vec[YDIM * 1];
	mul(C, x, C_vec, YDIM, ADIM, 1);

	// Compute the vector KC_vec = K*C_vec
	float KC_vec[ADIM * 1];
	mul(K, C_vec, KC_vec, ADIM, YDIM, 1);

	// Compute the vector Ky_vec = K*y
	float Ky_vec[ADIM * 1];
	mul(K, y, Ky_vec, ADIM, YDIM, 1);

	// Now add x = A_vec - KC_vec + B_vec + Ky_vec
	for (uint8_t i = 0; i < ADIM; i++) {
		x[i] = A_vec[i] - KC_vec[i] + B_vec[i] + Ky_vec[i];
	}
}
