/*
 * gpc.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This computes the Generalized Predictive Control inputs
 * Input: Matrix A, Matrix B, Matrix X, Vector x, Vector u, Vector r
 * Output: Vector u
 */
void gpc(float* A, float* B, float* C, float* x, float* u, float* r) {

	// Create the extended observability matrix
	float PHI[HORIZON*YDIM*ADIM];
	memset(PHI, 0, HORIZON*YDIM*ADIM*sizeof(float));
	obsv(PHI, A, C);

	// Create the lower triangular toeplitz matrix
	float GAMMA[HORIZON*YDIM*HORIZON*RDIM];
	memset(GAMMA, 0, HORIZON*YDIM*HORIZON*RDIM*sizeof(float));
	cab(GAMMA, PHI, A, B, C);

	// Find the input value from GAMMA and PHI
	// R_vec = R*r
	float R_vec[HORIZON * YDIM];
	memset(R_vec, 0, HORIZON * YDIM * sizeof(float));
	for (int i = 0; i < HORIZON * YDIM; i++) {
		for (int j = 0; j < YDIM; j++) {
			*(R_vec + i + j) = *(r + j);
		}
		i += YDIM - 1;
	}

	// PHI_vec = PHI*x
	float PHI_vec[HORIZON * YDIM];
	memset(PHI_vec, 0, HORIZON * YDIM * sizeof(float));
	mul(PHI, x, PHI_vec, HORIZON * YDIM, ADIM, 1);

	// R_PHI_vec = R_vec - PHI_vec
	float R_PHI_vec[HORIZON * YDIM];
	memset(R_PHI_vec, 0, HORIZON * YDIM*sizeof(float));
	for(int i = 0; i < HORIZON * YDIM; i++){
		*(R_PHI_vec + i) = *(R_vec + i) - *(PHI_vec + i);
	}

	// Solve R_vec from (GAMMAT*GAMMA + ALPHA*I)*R_vec = GAMMAT*R_PHI_vec
	// In this case, HORIZON * RDIM == HORIZON * YDIM
	linsolve_upper_triangular(GAMMA, R_vec, R_PHI_vec, HORIZON * YDIM);

	// Set first R_vec to u - Done
	for(int i = 0; i < RDIM; i++){
		*(u + i) = *(R_vec + i);
	}
}


