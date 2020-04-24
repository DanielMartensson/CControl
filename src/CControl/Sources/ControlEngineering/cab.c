/*
 * cab.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * Lower triangular toeplitz of extended observability matrix
 * CAB stands for C*A^i*B because every element is C*A*B
 */
void cab(float* GAMMA, float* PHI, float* A, float* B, float* C){

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
