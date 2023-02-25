/*
 * linsolve_qr.c
 *
 *  Created on: 10 juni 2021
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

/*
 * Solve Ax=b with QR decomposition
 * A[m*n]
 * b[m]
 * x[n]
 */
void linsolve_qr(float A[], float x[], float b[], uint16_t row, uint16_t column){
	// QR-decomposition
	float *Q = (float*)malloc(row * row * sizeof(float));
	float *R = (float*)malloc(row * column * sizeof(float));
	qr(A, Q, R, row, column, false);
	tran(Q, row, row); // Do transpose Q -> Q^T
	float *QTb = (float*)malloc(row * sizeof(float));
	mul(Q, b, QTb, row, row, 1); // Q^Tb = Q^T*b
	linsolve_upper_triangular(R, x, QTb, column);

	/* Free */
	free(Q);
	free(R);
	free(QTb);
}
