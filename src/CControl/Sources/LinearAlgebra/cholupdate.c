/*
 * cholupdate.c
 *
 *  Created on: 15 sep. 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Create L = cholupdate(L, x, rank_one_update)
 * L is a lower triangular matrix with real and positive diagonal entries from cholesky decomposition L = chol(A)
 * L [m*n]
 * x [m]
 * n == m
 */
void cholupdate(float L[], float x[], uint16_t row, bool rank_one_update){
	float alpha = 0.0, beta = 1.0, beta2 = 0.0, gamma = 0.0, delta = 0.0;
	tran(L, row, row);

	for(uint8_t i = 0; i < row; i++){
		alpha = x[i] / L[row*i + i];
		beta2 = rank_one_update == true ? sqrtf(beta * beta + alpha*alpha) : sqrtf(beta * beta - alpha*alpha);
		gamma = rank_one_update == true ? alpha / (beta2 * beta) : -alpha / (beta2 * beta);

		if(rank_one_update){
			// Update
			delta = beta / beta2;
			L[row*i + i] = delta * L[row*i + i] + gamma * x[i];

			if(i < row){
				for(uint8_t k = i+1; k < row; k++)// line 34 in tripfield chol_updown function
					x[k] -= alpha * L[row*k + i];

				for(uint8_t k = i+1; k < row; k++)// line 35 in tripfield chol_updown function
					L[row*k + i] = delta * L[row*k + i] + gamma * x[k];
			}
			x[i] = alpha;
			beta = beta2;
		}else{
			// Downdate
			delta = beta2 / beta;
			L[row*i + i] = delta * L[row*i + i];

			if(i < row){
				for(uint8_t k = i+1; k < row; k++)
					x[k] -= alpha * L[row*k + i];

				for(uint8_t k = i+1; k < row; k++)
					L[row*k + i] = delta * L[row*k + i] + gamma * x[k];
			}
			x[i] = alpha;
			beta = beta2;
		}
	}

	tran(L, row, row);
}
