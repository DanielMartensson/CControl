/*
 * opa.c
 *
 *  Created on: 4 december 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

/*
 * Ordinary Procrustes Analysis
 * X[m*n] - Reference
 * Y[m*n] - Raw data that are going to be transformed
 * Return the distance between X and transformed Y
 */
float opa(const float X[], float Y[], const size_t row, const size_t column) {
	/* Copy */
	const size_t row_column = row * column;
	const size_t total_bytes = row_column * sizeof(float);
	float* X0 = (float*)malloc(total_bytes);
	float* Y0 = (float*)malloc(total_bytes);
	memcpy(X0, X, total_bytes);
	memcpy(Y0, Y, total_bytes);

	/* Find the centroid */
	float* muX = (float*)malloc(column * sizeof(float));
	float* muY = (float*)malloc(column * sizeof(float));
	center(X0, muX, row, column);
	center(Y0, muY, row, column);

	/* Do Frobenius norm */
	float normX = norm(X0, row, column, NORM_METHOD_FROBENIUS);
	float normY = norm(Y0, row, column, NORM_METHOD_FROBENIUS);

	/* Scale to equal (unit) norm */
	scalar(X0, 1.0f / normX, row_column);
	scalar(Y0, 1.0f / normY, row_column);

	/* Turn X_copy to transpose */
	tran(X0, row, column);

	/* Multiply A = X' * Y */
	const size_t column_column = column * column;
	float* A = (float*)malloc(column_column * sizeof(float));
	mul(X0, Y0, A, column, row, column);

	/* Optimal rotation matrix of Y */
	float* U = (float*)malloc(column_column * sizeof(float));
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column_column * sizeof(float));
	float* T = (float*)malloc(column_column * sizeof(float));
	svd(A, column, column, U, S, V);
	tran(V, column, column);
	tran(U, column, column);
	mul(V, U, T, column, column, column);

	/* Find trace */
	float traceTA = 0.0f;
	sum(S, &traceTA, column, 1, true);
	
	/* Standardized distance between X and b*Y*T + c */
	const float d = 1.0f - traceTA * traceTA;

	/* Transformed coordinates */
	mul(Y0, T, Y, row, column, column);
	const float normX_traceTA = normX * traceTA;
	size_t i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			Y[j] *= normX_traceTA;
			Y[j] += muX[j];
		}
		Y += column;
	}

	/* Free */
	free(X0);
	free(Y0);
	free(muX);
	free(muY);
	free(A);
	free(U);
	free(S);
	free(V);
	free(T);

	/* Return distance d */
	return d;
}

/* GNU octave code
function [Y, d] = procrustes(X, Y)
	% Find the centroid
	muX = mean(X);
	muY = mean(Y);
	X0 = X - muX;
	Y0 = Y - muY;

	% Do Frobenius norm
	normX = norm(X0, 'fro');
	normY = norm(Y0, 'fro');

	% Scale to equal (unit) norm
	X0 = X0 / normX;
	Y0 = Y0 / normY;

	% Optimal rotation matrix of Y
	A = X0' * Y0;
	[U, S, Vt] = svd(A, 'econ');
	V = Vt';
	T = V * U';

	% Find trace
	traceTA = sum(diag(S));

	% Optimum scaling of Y
	b = traceTA * normX / normY;

	% Standardized distance between X and b*Y*T + c
	d = 1 - traceTA^2;

	% Transformed coordinates
	Y = Y0 * T;
	Y = normX * traceTA * Y;
	Y = Y + muX;

	% Transformation matrix
	c = muX - b * muY * T;

	% Transformation values
	tform = struct('rotation', T, 'scale', b, 'translation', c);
end
*/