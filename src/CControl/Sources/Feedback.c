/*
 * Prediction.c
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#include "../../CControl/Headers/Configurations.h"
#include "../../CControl/Headers/Functions.h"

#define MPC 1
#define GPC 0

/*
 * Consider these as private functions
 */
static void solve(float* GAMMA, float* PHI, float* x, float* u, float* r, int mode);
static void integral(float* xi, float* r, float* y, int anti_windup);
static void CAB(float* GAMMA, float* PHI, float* A, float* B, float* C);
static void obsv(float* PHI, float* A, float* C);

/*
 * This computes the Generlized Predictive Control inputs
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
	CAB(GAMMA, PHI, A, B, C);

	// Find the input value from GAMMA and PHI
	solve(GAMMA, PHI, x, u, r, GPC);
}

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
	CAB(GAMMA, PHI, A, B, C);

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

/*
 * This computes the Linear Quadratic Integral inputs
 * Input: Vector y, Vector u, Scalar qi, Vector r, Matrix L, Matrix Li, Vector x, Vecor xi, Option anti_windup
 */
void lqi(float* y, float* u, float qi, float* r, float* L, float* Li, float* x, float* xi, int anti_windup) {

	// First compute the L_vec = L*x
	float L_vec[RDIM*1];
	memset(L_vec, 0, RDIM*sizeof(float));
	mul(L, x, L_vec, RDIM, ADIM, 1);

	// Then compute the integral law Li_vec = Li*xi
	float Li_vec[RDIM];
	memset(Li_vec, 0, RDIM*sizeof(float));
	integral(xi, r, y, anti_windup);
	mul(Li, xi, Li_vec, RDIM, YDIM, 1);

	// u = Li/(1-qi)*r - (L*x - Li*xi)
	for(int i = 0; i < RDIM; i++){
		*(u + i) = *(Li + i*RDIM)/(1-qi) * *(r + i) - (*(L_vec + i) - *(Li_vec + i));
	}
}

/*
 * Model predictive control
 */
void mpc(float* A, float* B, float* C, float* x, float* u, float* r){
	// Create the extended observability matrix
	float PHI[HORIZON*YDIM*ADIM];
	memset(PHI, 0, HORIZON*YDIM*ADIM*sizeof(float));
	obsv(PHI, A, C);

	// Create the lower triangular toeplitz matrix
	float GAMMA[HORIZON*YDIM*HORIZON*RDIM];
	memset(GAMMA, 0, HORIZON*YDIM*HORIZON*RDIM*sizeof(float));
	CAB(GAMMA, PHI, A, B, C);

	// Find the input value from GAMMA and PHI
	solve(GAMMA, PHI, x, u, r, MPC);
}

/*
 * xi = xi + r - y;
 */
static void integral(float* xi, float* r, float* y, int anti_windup) {
	for(int i = 0; i < RDIM; i++){
		/*
		 * Anti-windup
		 */
		if(anti_windup == 0){
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate
		}else if(anti_windup == 1){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate when r > y, else delete
			}else{
				*(xi + i) = 0; // Delete just that xi
			}
		}else if(anti_windup == 2){
			if(*(r + i) > *(y + i)){
				*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Only integrate r > y, else stop
			}
		}else{
			*(xi + i) = *(xi + i) + *(r + i) - *(y + i); // Always integrate if nothing else selected
		}
	}
}


/*
 * Solve U from R = PHI*x + GAMMA*U
 * Hint: Look up lmpc.m in Matavecontrol
 */
static void solve(float* GAMMA, float* PHI, float* x, float* u, float* r, int mode) {

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
	if(mode == GPC){
		linsolve(GAMMA, R_vec, R_PHI_vec, HORIZON * YDIM, HORIZON * RDIM);
	}else if(mode == MPC){
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

		// A = GAMMAT*GAMMA
		float A[HORIZON * RDIM*HORIZON * RDIM];
		memset(A, 0, HORIZON * RDIM*HORIZON * RDIM * sizeof(float));
		mul(GAMMAT, GAMMA, A, HORIZON * RDIM, HORIZON*YDIM, HORIZON * RDIM);

		// A = A + alpha*I
		for(int i = 0; i < HORIZON * RDIM; i++){
			*(A + i*HORIZON * RDIM + i) = *(A + i*HORIZON * RDIM + i) + ALPHA; // Don't need identity matrix here because we only add on diagonal
		}

		// Copy A and call it AT
		float AT[HORIZON * RDIM*HORIZON * RDIM];
		memcpy(AT, A, HORIZON * RDIM*HORIZON * RDIM*sizeof(float)); // A -> AT
		tran(AT, HORIZON*RDIM, HORIZON*RDIM);

		// Now create c = AT*R_PHI_vec
		float c[HORIZON * YDIM];
		memset(c, 0, HORIZON * YDIM*sizeof(float));
		mul(AT, R_PHI_vec, c, HORIZON*RDIM, HORIZON*RDIM, 1);

		// Do linear programming now
		linprog(c, A, b, R_vec, HORIZON*YDIM, HORIZON*RDIM, ITERATION_LIMIT);
		//print(R_vec, HORIZON * YDIM, 1);

	}
	// Set first R_vec to u
	for(int i = 0; i < RDIM; i++){
		*(u + i) = *(R_vec + i);
	}

}

/*
 * Lower traingular toeplitz of extended observability matrix
 */
void CAB(float* GAMMA, float* PHI, float* A, float* B, float* C){

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

/*
 * [C*A^1; C*A^2; C*A^3; ... ; C*A^HORIZON] % Extended observability matrix
 */
void obsv(float* PHI, float* A, float* C) {

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

