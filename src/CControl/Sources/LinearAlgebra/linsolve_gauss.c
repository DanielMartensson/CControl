/*
 * linsolve_gauss.c
 *
 *  Created on: 10 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

static void triu(float A[], float b[], size_t row);
static void tikhonov(float A[], float b[], float ATA[], float ATb[], size_t row_a, size_t column_a, float alpha);

/*
 * This is gaussian elemination
 * If alpha = 0:
 * A[m*n]
 * b[m]
 * x[n]
 * m == n
 * If alpha > 0: (Tikhonov regularization is used)
 * A[m*n]
 * b[m]
 * x[n]
 * m =/= n
 */
void linsolve_gauss(float A[], float x[], float b[], size_t row, size_t column, float alpha){
	/* Solve now */
	if(alpha <= MIN_VALUE && row == column){
		/* Create two copies of A and b */
		float *Ac = (float*)malloc(row * column * sizeof(float));
		memcpy(Ac, A, row*column*sizeof(float));
		float *bc = (float*)malloc(column * sizeof(float));
		memcpy(bc, b, column*sizeof(float));

		/* Solve */
		triu(Ac, bc, row);
		linsolve_upper_triangular(Ac, x, bc, column);
		
		/* Free */
		free(Ac);
		free(bc);
	}else{
		/* x = inv(A'*A + lambda*I)*A'*b */
		float *ATA = (float*)malloc(column * column * sizeof(float));
		float *ATb = (float*)malloc(column * sizeof(float));
		tikhonov(A, b, ATA, ATb, row, column, alpha);
		triu(ATA, ATb, column);
		linsolve_upper_triangular(ATA, x, ATb, column);

		/* Free */
		free(ATA);
		free(ATb);
	}
}


/*
 * Prepare A and b into upper triangular with triu.
 * A [m*n] A need to be square
 * b [m]
 * n == m
 */
static void triu(float A[], float b[], size_t row) {
	/* Save address */
	float *Ai;
	float *Aj = A;

	/* Make A to upper triangular. Also change b as well */
	float pivot;
	size_t i, j, k;
	for(j = 0; j < row; j++){ /* Column */
		Ai = A;
		for(i = 0; i < row; i++){ /* row */
			if(i > j){
				pivot = Ai[j] / Aj[j];
				/* pivot = A[i*row + j] / A[j*row + j]; */
				for (k = 0; k < row; k++) {
					Ai[k] -= pivot * Aj[k];
					/* A[i*row + k] = A[i*row + k] - pivot * A[j*row + k]; */
				}
				b[i] = b[i] - pivot * b[j];
			}
			Ai += row;
		}
		Aj += row;
	}
}

/*
 * GNU Octave code:
 *  function [A, b] = backsubstitution(A, b)
	  s = length(A);
	  for j = 1:(s-1)
		  for i = s:-1:j+1
			  m = A(i,j)/A(j,j);
			  A(i,:) = A(i,:) - m*A(j,:);
			  b(i) = b(i) - m*b(j);
		  end
	  end
	end
 */

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
static void tikhonov(float A[], float b[], float ATA[], float ATb[], size_t row_a, size_t column_a, float alpha){
	/* AT - Transpose A */
	float *AT = (float*)malloc(column_a * row_a * sizeof(float)); /* Same dimension as A, just swapped rows and column */
	memcpy(AT, A, column_a*row_a*sizeof(float)); /* Copy A -> AT */
	tran(AT, row_a, column_a); /* Now turn the values of AT to transpose */

	/* ATb = AT*b */
	memset(ATb, 0, column_a * sizeof(float));
	mul(AT, b, ATb, column_a, row_a, 1);

	/* ATA = AT*A */
	memset(ATA, 0, column_a*column_a * sizeof(float));
	mul(AT, A, ATA, column_a, row_a, column_a);

	/* ATA = ATA + alpha*I. Don't need identity matrix here because we only add on diagonal */
	float *ATA0 = ATA;
	size_t i;
	for(i = 0; i < column_a; i++){
		ATA0[i] += alpha;
		ATA0 += column_a;
		/* ATA[i*column_a + i] = ATA[i*column_a + i] + alpha; */
	}
	/* Now we have our ATA = (A ^ T * A + alpha * I) and ATb = A ^ T * b */

	/* Free */
	free(AT);
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
