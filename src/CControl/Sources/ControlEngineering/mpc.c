/*
 * mpc.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * Model predictive control
 * Hint: Look up lmpc.m in Matavecontrol
 */
void mpc(float* A, float* B, float* C, float* x, float* u, float* r){
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

	// Find the input value from GAMMA and PHI
	/* Find the optimal solution R_vec from linear programming
	 * min (GAMMAT*GAMMA + ALPHA*I)^T*R_PHI_vec*R_vec
	 * s.t
	 * 		(GAMMAT*GAMMA + ALPHA*I)*R_vec <= GAMMAT*R_PHI_vec
	 *								 R_vec >= 0
	 */

	// Transpose gamma
	float GAMMAT[HORIZON*YDIM*HORIZON*RDIM];
	memcpy(GAMMAT, GAMMA, HORIZON*YDIM*HORIZON*RDIM*sizeof(float)); // GAMMA -> GAMMAT
	tran(GAMMAT, HORIZON*YDIM, HORIZON*RDIM);

	// b = GAMMAT*R_PHI_vec
	float b[HORIZON * YDIM];
	memset(b, 0, HORIZON * YDIM * sizeof(float));
	mul(GAMMAT, R_PHI_vec, b, HORIZON * RDIM, HORIZON*YDIM, 1);

	// GAMMATGAMMA = GAMMAT*GAMMA
	float GAMMATGAMMA[HORIZON * RDIM*HORIZON * RDIM];
	memset(GAMMATGAMMA, 0, HORIZON * RDIM*HORIZON * RDIM * sizeof(float));
	mul(GAMMAT, GAMMA, GAMMATGAMMA, HORIZON * RDIM, HORIZON*YDIM, HORIZON * RDIM);

	// A = A + alpha*I
	for(int i = 0; i < HORIZON * RDIM; i++){
		*(GAMMATGAMMA + i*HORIZON * RDIM + i) = *(GAMMATGAMMA + i*HORIZON * RDIM + i) + ALPHA; // Don't need identity matrix here because we only add on diagonal
	}

	// Copy A and call it AT
	float AT[HORIZON * RDIM*HORIZON * RDIM];
	memcpy(AT, GAMMATGAMMA, HORIZON * RDIM*HORIZON * RDIM*sizeof(float)); // A -> AT
	tran(AT, HORIZON*RDIM, HORIZON*RDIM);

	// Now create c = AT*R_PHI_vec
	float c[HORIZON * YDIM];
	memset(c, 0, HORIZON * YDIM*sizeof(float));
	mul(AT, R_PHI_vec, c, HORIZON*RDIM, HORIZON*RDIM, 1);

	// Do linear programming now
	// TODO: Fixa så att man kan ha max och min på y
	linprog(c, A, b, R_vec, HORIZON*YDIM, HORIZON*RDIM, 1, ITERATION_LIMIT);
	//print(R_vec, HORIZON * YDIM, 1);

	// Set first R_vec to u - Done
	for(int i = 0; i < RDIM; i++){
		*(u + i) = *(R_vec + i);
	}
}


