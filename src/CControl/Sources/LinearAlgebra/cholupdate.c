/*
 * cholupdate.c
 *
 *  Created on: 15 sep. 2021
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

/*
 * Create L = cholupdate(L, x, rank_one_update)
 * L is a lower triangular matrix with real and positive diagonal entries from cholesky decomposition L = chol(A)
 * L [m*n]
 * x [m]
 * n == m
 */
void cholupdate(float L[], float x[], const size_t row, const bool rank_one_update){
	/* Decleration */
	size_t i, k;

	/* Save address */
	float *Li = L;
	float *Lk;

	float alpha, beta2, gamma, delta = 0.0f;
	float beta = 1.0f;
	tran(L, row, row);

	for(i = 0; i < row; i++){
		alpha = x[i] / Li[i];
		/* alpha = x[i] / L[row*i + i]; */

		beta2 = rank_one_update == true ? sqrtf(beta * beta + alpha*alpha) : sqrtf(beta * beta - alpha*alpha);
		gamma = rank_one_update == true ? alpha / (beta2 * beta) : -alpha / (beta2 * beta);

		if(rank_one_update){
			/* Update */
			delta = beta / beta2;
			Li[i] = delta * Li[i] + gamma * x[i];
			/* L[row*i + i] = delta * L[row*i + i] + gamma * x[i]; */

			if(i < row - 1){
				Lk = L;
				Lk += row*i; /* Important that it is row*i and not row*(i+1) */
				for(k = i+1; k < row; k++){ /* line 34 in tripfield chol_updown function */
					Lk += row; /* Here is the +1 */
					x[k] -= alpha * Lk[i];
					Lk[i] = delta * Lk[i] + gamma * x[k];
					/* x[k] -= alpha * L[row*k + i]; */
					/* L[row*k + i] = delta * L[row*k + i] + gamma * x[k]; */
				}
			}
			x[i] = alpha;
			beta = beta2;
		}else{
			/* Downdate */
			delta = beta2 / beta;
			Li[i] = delta * Li[i];
			/* L[row*i + i] = delta * L[row*i + i]; */

			if(i < row - 1){
				Lk = L;
				Lk += row*i; /* Important that it is row*i and not row*(i+1) */
				for(k = i+1; k < row; k++){
					Lk += row; /* Here is the +1 */
					x[k] -= alpha * Lk[i];
					Lk[i] = delta * Lk[i] + gamma * x[k];
					/* x[k] -= alpha * L[row*k + i] */
					/* L[row*k + i] = delta * L[row*k + i] + gamma * x[k]; */
				}
			}
			x[i] = alpha;
			beta = beta2;
		}

		Li += row;
	}

	tran(L, row, row);
}