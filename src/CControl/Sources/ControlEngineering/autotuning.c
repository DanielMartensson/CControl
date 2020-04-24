/*
 * autotuning.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This computes the unconstrained Model Predictive Control control law L and its reference gain Kr
 * Input: Matrix A, Matrix B, Matrix C, Matrix L, Matrix Kr
 * Output: Matrix L, Matrix Kr
 */
void autotuning(float* A, float* B, float* C, float* L, float* Kr){

	// Create the extended observability matrix
	float PHI[HORIZON*YDIM*ADIM];
	memset(PHI, 0, HORIZON*YDIM*ADIM*sizeof(float));
	obsv(PHI, A, C);

	// Create the lower triangular toeplitz matrix
	float GAMMA[HORIZON*YDIM*HORIZON*RDIM];
	memset(GAMMA, 0, HORIZON*YDIM*HORIZON*RDIM*sizeof(float));
	cab(GAMMA, PHI, A, B, C);

	/* MPC Equation: U = (GAMMA'GAMMA + ALPHA*I)^{-1}GAMMA'(R*r-PHI*x)
	 * L = -(GAMMA'GAMMA + ALPHA*I)^{-1}GAMMA'PHI*x
	 * Kr = (GAMMA'GAMMA + ALPHA*I)^{-1}GAMMA'*R*r
	 *
	 * But we will do - because inv calls() linsolve() and linsolve do (GAMMA'GAMMA + ALPHA*I)^{-1}GAMMA' to inv(GAMMA)
	 * L = -inv(GAMMA)*PHI
	 * Kr = inv(GAMMA)*R
	 */

	// IGAMMA = inv(GAMMA) Turn it to inverse
	inv(GAMMA, HORIZON*RDIM);

	// IGAMMAPHI = IGAMMA*PHI
	float IGAMMAPHI[HORIZON*RDIM*ADIM];
	memset(IGAMMAPHI, 0, HORIZON*YDIM*ADIM*sizeof(float));
	mul(GAMMA, PHI, IGAMMAPHI, HORIZON*YDIM, HORIZON*RDIM, ADIM);

	// Create R vector with ones
	float R[HORIZON*RDIM*YDIM]; // Vector, not matrix
	memset(R, 0, HORIZON*RDIM*YDIM*sizeof(float));
	for(int i = 0; i < HORIZON*RDIM*YDIM; i++)
		*(R + i) = 1;

	// IGAMMAR = IGAMMA*R
	float IGAMMAR[HORIZON*YDIM*RDIM];
	memset(IGAMMAR, 0, HORIZON*YDIM*RDIM*sizeof(float));
	mul(GAMMA, R, IGAMMAR, HORIZON*YDIM, HORIZON*RDIM, YDIM);

	// IGAMMAPHI to L
	memset(L, 0, RDIM*ADIM*sizeof(float));
	memcpy(L, IGAMMAPHI, RDIM*ADIM*sizeof(float));

	// IGAMMAR to Kr
	memset(Kr, 0, RDIM*sizeof(float));
	memcpy(Kr, IGAMMAR, RDIM*sizeof(float));

}


