/*
 * mpc.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

static void obsv(float* PHI, float* A, float* C);
static void cab(float* GAMMA, float* PHI, float* A, float* B, float* C);

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


/*
 * [C*A^1; C*A^2; C*A^3; ... ; C*A^HORIZON] % Extended observability matrix
 */
static void obsv(float* PHI, float* A, float* C) {

	// This matrix will A^(i+1) all the time
	float A_pow[ADIM*ADIM];
	memset(A_pow, 0, ADIM * ADIM * sizeof(float));
	float A_copy[ADIM*ADIM];
	memcpy(A_copy, A, ADIM * ADIM * sizeof(float));

	// Temporary matrix
	float T[YDIM*ADIM];
	memset(T, 0, YDIM * ADIM * sizeof(float));

	// Regular T = C*A^(1+i)
	mul(C, A, T, YDIM, ADIM, ADIM);

	// Insert temporary T into PHI
	memcpy(PHI, T, YDIM*ADIM*sizeof(float));

	// Do the rest C*A^(i+1) because we have already done i = 0
	for(int i = 1; i < HORIZON; i++){
		mul(A, A_copy, A_pow, ADIM, ADIM, ADIM); //  Matrix power A_pow = A*A_copy
		mul(C, A_pow, T, YDIM, ADIM, ADIM); // T = C*A^(1+i)
		memcpy(PHI + i*YDIM*ADIM, T, YDIM*ADIM*sizeof(float)); // Insert temporary T into PHI
		memcpy(A_copy, A_pow, ADIM * ADIM * sizeof(float)); // A_copy <- A_pow
	}
}

/*
 * Lower triangular toeplitz of extended observability matrix
 * CAB stands for C*A^i*B because every element is C*A*B
 */
static void cab(float* GAMMA, float* PHI, float* A, float* B, float* C){

	// First create the initial C*A^0*B == C*I*B == C*B
	float CB[YDIM*RDIM];
	memset(CB, 0, YDIM*RDIM*sizeof(float));
	mul(C, B, CB, YDIM, ADIM, RDIM);

	// Take the transpose of CB so it will have dimension RDIM*YDIM instead
	tran(CB, YDIM, RDIM);

	// Create the CAB matrix from PHI*B
	float PHIB[HORIZON*YDIM*RDIM];
	mul(PHI, B, PHIB, HORIZON*YDIM, ADIM, RDIM); // CAB = PHI*B
	tran(PHIB, HORIZON*YDIM, RDIM);

	/*
	 * We insert GAMMA = [CB PHI;
	 *                    0  CB PHI;
	 *            		  0   0  CB PHI;
	 *            		  0   0   0  CB PHI] from left to right
	 */
	for(int i = 0; i < HORIZON; i++) {
		for(int j = 0; j < RDIM; j++) {
			memcpy(GAMMA + HORIZON*YDIM*(i*RDIM+j) + YDIM*i, CB + YDIM*j, YDIM*sizeof(float)); // Add CB
			memcpy(GAMMA + HORIZON*YDIM*(i*RDIM+j) + YDIM*i + YDIM, PHIB + HORIZON*YDIM*j, (HORIZON-i-1)*YDIM*sizeof(float)); // Add PHI*B
		}
	}

	// Transpose of gamma
	tran(GAMMA, HORIZON*RDIM, HORIZON*YDIM);

}

