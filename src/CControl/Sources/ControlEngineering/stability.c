/*
 * stability.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Configurations.h"
#include "../../Headers/Functions.h"

/*
 * This uses the dlyap function to check the stability
 * A need to be square. Returns 0 when system is not stable.
 * A is from the dx = Ax + Bu system
 * Returns 1 when system is stable.
 */
int stability(float* A){

	// TODO: Här måste vi ha precis som kalman.c har.
	// En if-sats för att dela upp A om det är integration på
	float P[ADIM*ADIM];

	// Create a symmetric matrix that is a positive definite matrix
	float Q[ADIM*ADIM];
	for(int i = 0; i < ADIM; i++)
		for(int j = 0; j < ADIM; j++)
			P[ADIM*i + j] = i + j + 1;
	dlyap(A, P, Q, ADIM);

	// Check if P is symmetric and positive definite
	for(int i = 0; i < ADIM; i++){
		for(int j = i; j < ADIM; j++){
			if(i == j){
				if(P[ADIM*i + j] <= 0) return 0; // Diagonal
			}else{
				if(P[ADIM*i + j] <= 0 && P[ADIM*j + i] <= 0) return 0; // non-Diagonal
			}
		}
	}

	return 1;
}


