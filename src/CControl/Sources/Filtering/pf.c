/*
 * pf.c
 *
 *  Created on: 31 okt. 2022
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

static void shift_matrix(float matrix[], float x[], uint8_t p, uint8_t *k, uint8_t m);
static void kernel_density_estimation(float P[], float H[], float horizon[], float noise[], uint8_t m, uint8_t p);

void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], uint8_t m, uint8_t p, uint8_t *k){
	// Horizon matrix shifting
	shift_matrix(horizon, x, p, k, m);

	// Compute the kernel density estimation from horizon
	float P[m*p];
	float H[m*p];
	kernel_density_estimation(P, H, horizon, noise, m, p);

}

static void shift_matrix(float matrix[], float x[], uint8_t p, uint8_t *k, uint8_t m){
	// Matrix shifting
	for(uint8_t i = 0; i < m; i++){
		// Shift
		if(*k >= p	){
			matrix[i*p + p-2] = matrix[i*p + p-1];
		}

		// Add the last
		matrix[i*p + *k] = x[i];
	}
}

static void kernel_density_estimation(float P[], float H[], float horizon[], float noise[], uint8_t m, uint8_t p){
	// Sort smallest values first because H is like a histogram with only one count for each values

}
