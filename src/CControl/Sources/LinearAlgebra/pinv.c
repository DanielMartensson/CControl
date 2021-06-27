/*
 * pinv.c
 *
 *  Created on: 13 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Pseudo inverse by using Singular Value Decomposition
 * A [m*n]
 * m >= n
 * A+ = V*inv(S)*U'
 * This return matrix A [n*m] - Reversed size
 */
void pinv(float A[], uint16_t row, uint16_t column){

	// Use Golub and Reinch if row != column
	float U[row*column];
	float S[column];
	float V[column*column];
	if(row == column)
		svd_jacobi_one_sided(A, row, MAX_ITERATION_COUNT_SVD, U, S, V);
	else
		svd_golub_reinsch(A, row, column, U, S, V);

	// Do inv(S)
	for(uint16_t i = 0; i < column; i++)
		S[i] = 1.0/S[i]; // Create inverse diagonal matrix

	// Transpose U'
	tran(U, row, column);

	// U = S*U'
	for(uint16_t i = 0; i < row; i++)
		for(uint16_t j = 0; j < column; j++)
			U[row*j + i] = S[j]*U[row*j + i];

	// Do pinv now: A = V*U
	mul(V, U, A, column, column, row);

}

