/*
 * qmpc.c
 *
 *  Created on: 28:e Juli mars 2024
 *      Author: Daniel Mårtensson
 */

#include "controlengineering.h"


/*
 * Model predictive control with quadratic programming and constraints on outputs and inputs
 * Hint: Look up qmpc.m in Matavecontrol
 * 
 * GAMMA[(N*row_c)*(N*column_b)]
 * PHI[(N*row_c)*row_a]
 * x[row_a]
 * u[column_b]
 * Umax[column_b]
 * r[row_c]
 */
void qmpc(const float GAMMA[], const float PHI[], const float x[], float u[], const float Umax[], const float r[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N, const float lambda, const bool has_integration_action, const float integration_constant) {
	/*
	% Solve: R = PHI*x + GAMMA*U with quadratic programming: Min: 1/2x^TQx + c^Tx, S.t: Ax <= b, x >= 0
    % Q = a*eye(size(GAMMA))
    % cqp = GAMMA'*Q*(PHI*x - R)
    % bqp = [R - PHI*x; Umax; 0]
    % aqp = [GAMMA; I; -I]
    % qqp = GAMMA'*GAMMA + Q
	*/
	
	/* Create R vector */
	float* R = (float*)malloc(N * row_c * sizeof(float));
	size_t i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < row_c; j++) {
			R[i * row_c + j] = r[j];
		}
	}

	/* Create PHIx vector */
	float* PHIx = (float*)malloc(N * row_c * sizeof(float));
	mul(PHI, x, PHIx, N * row_c, row_a, 1);

	/* Create bqp = [R - PHIx; Umax; Umax*0] vector */
	float* bqp = malloc((N * row_c + N * column_b + N * column_b) * sizeof(float));
	memset(bqp, 0, (N * row_c + N * column_b + N * column_b) * sizeof(float));
	for(i = 0; i < N * row_c; i++) {
		bqp[i] = R[i] - PHIx[i];
	}
	for (i = N * row_c; i < N * row_c + N; i++) {
		for (j = 0; j < column_b; j++) {
			bqp[i * column_b + j] = Umax[j];
		}
	}

	/* Transpose GAMMA */
	float* GAMMAT = (float*)malloc((N * row_c) * (N * column_b) * sizeof(float));
	memcpy(GAMMAT, GAMMA, (N * row_c) * (N * column_b) * sizeof(float));
	tran(GAMMAT, N * row_c, N * column_b);

	/* Create qqp = GAMMAT * GAMMA matrix */
	float* qqp = (float*)malloc(N * column_b * N * column_b * sizeof(float));
	mul(GAMMAT, GAMMA, qqp, N * column_b, N * row_c, N * column_b);

	/* Add regularization to qqp matrix */
	for (i = 0; i < N * column_b; i++) {
		qqp[i * N * column_b + i] += lambda;
	}

	/* Create aqp = [GAMMA; I; -I] */
	float* aqp = (float*)malloc((N * row_c + N * column_b + N * column_b) * (N * column_b) * sizeof(float));
	memset(aqp, 0, (N * row_c + N * column_b + N * column_b) * (N * column_b) * sizeof(float));
	memcpy(aqp, GAMMA, (N * row_c) * (N * column_b) * sizeof(float));
	for (i = 0; i < N * column_b; i++) {
		aqp[i * (N * column_b) + i + (N * row_c) * (N * column_b)] = 1.0f;
		aqp[i * (N * column_b) + i + (N * row_c + N * column_b) * (N * column_b)] = -1.0f;
	}

	/* Create PHIx_R vector */
	float* PHIx_R = (float*)malloc(N * row_c * sizeof(float));
	for (i = 0; i < N * row_c; i++) {
		PHIx_R[i] = PHIx[i] - R[i];
	}

	/* Create cqp vector */
	float* cqp = (float*)malloc(N * column_b * sizeof(float));
	mul(GAMMAT, PHIx_R, cqp, N * column_b, N * row_c, 1);
	scalar(cqp, lambda, N * column_b);

	/* Find the optimal solution with quadprog - We are using R as the output, instead of u */
	quadprog(qqp, cqp, aqp, bqp, NULL, NULL, R, N * row_c + N * column_b + N * column_b, 0, N * column_b, false);

	/* We select the best input values, depending on if we have integration behavior or not in our model */
	if(has_integration_action){
		/* Set first R to u */
		for(i = 0; i < column_b; i++) {
			u[i] = R[i] - integration_constant * u[i];
		}
	}else{
		/* Set last R to u */
		for(i = 0; i < column_b; i++) {
			u[i] = R[N * column_b - column_b + i];
		}
	}

	/* Free */
	free(R);
	free(PHIx);
	free(GAMMAT);
	free(bqp);
	free(qqp);
	free(cqp);
	free(aqp);
	free(PHIx_R);
}