/*
 * qmpc.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"


/*
 * Model predictive control with linear programming
 * Hint: Look up qmpc.m in Matavecontrol
 */
void qmpc(float GAMMA[], float PHI[], float x[], float u[], float r[], size_t row_a, size_t row_c, size_t column_b, size_t N, bool has_integration) {
	
	
	
	/* TODO: This is under development 
	
	// Create the extended observability matrix
	float PHI[HORIZON*YDIM*ADIM];
	obsv(PHI, A, C, ADIM, YDIM, HORIZON);

	// Create the lower triangular toeplitz matrix
	float GAMMA[HORIZON*YDIM*HORIZON*RDIM];
	memset(GAMMA, 0, HORIZON*YDIM*HORIZON*RDIM*sizeof(float)); // We need memset here
	cab(GAMMA, PHI, B, C, ADIM, YDIM, RDIM, HORIZON);

	// Find the input value from GAMMA and PHI
	// R_vec = R*r
	float R_vec[HORIZON * YDIM];
	for (size_t i = 0; i < HORIZON * YDIM; i++) {
		for (size_t j = 0; j < YDIM; j++) {
			R_vec[i + j] = r[j];
		}
		i += YDIM - 1;
	}

	// PHI_vec = PHI*x
	float PHI_vec[HORIZON * YDIM];
	mul(PHI, x, PHI_vec, HORIZON * YDIM, ADIM, 1);

	// R_PHI_vec = R_vec - PHI_vec
	float R_PHI_vec[HORIZON * YDIM];
	for(size_t i = 0; i < HORIZON * YDIM; i++){
		*(R_PHI_vec + i) = *(R_vec + i) - *(PHI_vec + i);
	}

	// Transpose gamma
	float GAMMAT[HORIZON*YDIM*HORIZON*RDIM];
	memcpy(GAMMAT, GAMMA, HORIZON*YDIM*HORIZON*RDIM*sizeof(float)); // GAMMA -> GAMMAT
	tran(GAMMAT, HORIZON*YDIM, HORIZON*RDIM);

	// b = GAMMAT*R_PHI_vec
	float b[HORIZON * YDIM];
	mul(GAMMAT, R_PHI_vec, b, HORIZON * RDIM, HORIZON*YDIM, 1);

	// GAMMATGAMMA = GAMMAT*GAMMA = A
	float GAMMATGAMMA[HORIZON * RDIM*HORIZON * RDIM];
	mul(GAMMAT, GAMMA, GAMMATGAMMA, HORIZON * RDIM, HORIZON*YDIM, HORIZON * RDIM);

	// Copy A and call it AT
	float AT[HORIZON * RDIM*HORIZON * RDIM];
	memcpy(AT, GAMMATGAMMA, HORIZON * RDIM*HORIZON * RDIM*sizeof(float)); // A -> AT
	tran(AT, HORIZON*RDIM, HORIZON*RDIM);

	// Now create c = AT*R_PHI_vec
	float c[HORIZON * YDIM];
	mul(AT, R_PHI_vec, c, HORIZON*RDIM, HORIZON*RDIM, 1);

	// Do linear programming now
	linprog(c, GAMMATGAMMA, b, R_vec, HORIZON*YDIM, HORIZON*RDIM, 0);

	// We select the best input values, depending on if we have integration behavior or not in our model
	if(has_integration == true){
		// Set first R_vec to u - Done
		for(size_t i = 0; i < RDIM; i++){
			u[i] = R_vec[i];
		}
	}else{
		// Set last R_vec to u - Done
		for(size_t i = 0; i < RDIM; i++){
			u[i] = R_vec[HORIZON * RDIM - RDIM + i];
		}
	}
	*/
}