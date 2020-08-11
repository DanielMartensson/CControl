/*
 * tikhonov.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * This is Tikhonov regularization.
 * This function prepare your Ax = b equation to be solved as (A^T*A + alpha*I)*x = A^T*b
 * Use triu and then linsolve_upper_triangular after this function.
 *
 * A [m*n]
 * b [m]
 * ATA [n*n]
 * ATb [n]
 * alpha > 0, but very small number
 * m = row
 * n = column
 */
void tikhonov(float* A, float* b, float* ATA, float* ATb, int row_a, int column_a, float alpha){
	// AT - Transpose A
	float AT[column_a*row_a]; // Same dimension as A, just swapped rows and column
	memcpy(AT, A, column_a*row_a*sizeof(float)); // Copy A -> AT
	tran(AT, row_a, column_a); // Now turn the values of AT to transpose

	// ATb = AT*b
	memset(ATb, 0, row_a * sizeof(float));
	mul(AT, b, ATb, column_a, row_a, 1);

	// ATA = AT*A
	memset(ATA, 0, column_a*column_a * sizeof(float));
	mul(AT, A, ATA, column_a, row_a, column_a);

	// ATA = ATA + alpha*I. Don't need identity matrix here because we only add on diagonal
	for(int i = 0; i < column_a; i++){
		*(ATA + i*column_a + i) = *(ATA + i*column_a + i) + alpha;
	}
	// Now we have our ATA = (A^T*A + alpha*I) and ATb = A^T*b

}

/*
 * GNU Octave code:
 *  >> A = [3 4 5; 3 5 6; 6 7 8; 3 5 6];
	>> b = [2 ; 3 ; 5; 6];
	>> ATA = (A'*A + 0.01*eye(3))
	ATA =

		63.010    84.000    99.000
		84.000   115.010   136.000
		99.000   136.000   161.010

	>> ATb = A'*b
	ATb =

		63
		88
	   104

	>>
 */
