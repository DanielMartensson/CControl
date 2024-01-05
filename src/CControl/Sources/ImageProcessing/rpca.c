/*
 * rpca.c
 *
 *  Created on: 22 april 2023
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

static float do_Frobenius_on_A(float A[], float X[], float L[], float S[], size_t row, size_t column);
static void SVT(float A[], float X[], float S[], float Y[], float L[], float tau, size_t row, size_t column);
static void shrink_matrix(float A[], float X[], float tau, size_t row, size_t column);

/*
 * Robust principal component analysis
 * Use this if you want to remove outliers of an image X
 * X = L + S
 * X[m*n] - Image
 * L[m*n] - No outliers
 * S[m*n] - Outliers
 */
void rpca(float X[], float L[], float S[], size_t row, size_t column) {
	/* Get two tuning factors */
	uint32_t i, row_column = row * column;
	float sum_abs_X = 0;
	for (i = 0; i < row_column; i++) {
		sum_abs_X += fabsf(X[i]);
	}
	float mu = ((float)row_column) / (4.0f * sum_abs_X);
	float lambda = 1.0f / sqrtf(vmax(row, column));
	
	/* Create a threshold with Frobenius norm */
	float thresh = 0.000001f * norm(X, row, column, NORM_METHOD_FROBENIUS);

	/* Compute tau and alpha */
	float tau = 1.0f / mu;
	float alpha = lambda / mu;

	/* Create only these matrices - They are slack matrices */
	float* A = (float*)malloc(row_column * sizeof(float));
	float* Y = (float*)malloc(row_column * sizeof(float));

	/* Clear */
	memset(Y, 0, row_column * sizeof(float));
	memset(S, 0, row_column * sizeof(float));

	/* Start optimization */
	size_t count = 0;
	while (do_Frobenius_on_A(A, X, L, S, row, column) > thresh || count < 1000) {
		SVT(A, X, S, Y, L, tau, row, column);
		for (i = 0; i < row_column; i++) {
			A[i] = X[i] - L[i] + tau * Y[i];
		}
		shrink_matrix(S, A, alpha, row, column);
		for (i = 0; i < row_column; i++) {
			Y[i] = Y[i] + mu * (X[i] - L[i] - S[i]);
		}
		count++;
	}

	/* Free */
	free(A);
	free(Y);
}

static float do_Frobenius_on_A(float A[], float X[], float L[], float S[], size_t row, size_t column) {
	uint32_t i, row_column = row * column;
	for (i = 0; i < row_column; i++) {
		A[i] = X[i] - L[i] - S[i];
	}
	return norm(A, row, column, NORM_METHOD_FROBENIUS);
}

static void SVT(float A[], float X[], float S[], float Y[], float L[], float tau, size_t row, size_t column) {
	/* Do SVD */
	size_t i, row_column = row * column;
	for (i = 0; i < row_column; i++) {
		A[i] = X[i] - S[i] + tau*Y[i];
	}

	float* U = (float*)malloc(row_column * sizeof(float));
	float* E = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	svd(A, row, column, U, E, V);

	/* Call shrink, a special function */
	shrink_matrix(E, E, tau, 1, column);
	
	/* Transpose V */
	tran(V, column, column);

	/* Multiplying corresponding elements V = E.*V */
	size_t j;
	float* V0 = V;
	for (j = 0; j < column; j++) {
		V = V0;
		for (i = 0; i < column; i++) {
			V[j] *= E[i];
			/* Next row */
			V += column;
		}
	}

	/* Reset */
	V = V0;

	/* Multiply U*V = L */
	mul(U, V, L, row, column, column);

	/* Free */
	free(U);
	free(E);
	free(V);
}

static void shrink_matrix(float A[], float X[], float tau, size_t row, size_t column) {
	uint32_t i, row_column = row * column;
	for (i = 0; i < row_column; i++) {
		A[i] = sign(X[i]) * vmax(fabsf(X[i]) - tau, 0);
	}
}

/* GNU Octave code:
	% Robust Principal Component Analysis
	% Input: data matrix(X)
	% Output: filtred data matrix(L), sparse noise matrix(S)
	% Example 1: [L, S] = rpca(X)
	% Author: Daniel Mårtensson, 28 April 2021.

	function [L, S] = rpca(varargin)
	 % Check if there is any input
	  if(isempty(varargin))
		error('Missing imput')
	  end

	  % Get X
	  X = varargin{1};

	  % Get size of the matrix X
	  [n1, n2] = size(X);

	  % Get two tuning factors
	  mu = n1*n2/(4*sum(abs(X(:))));
	  lambda = 1/sqrt(max(n1, n2));

	  % Create a threshold
	  thresh = 1e-6*norm(X, 'fro');

	  % Create three matrices
	  L = zeros(size(X));
	  S = zeros(size(X));
	  Y = zeros(size(X));

	  % Start optimization
	  count = 0;
	  while(and(norm(X-L-S, 'fro') > thresh, count < 1000))
		L = SVT(X-S+(1/mu)*Y, 1/mu);
		S = shrink(X-L+(1/mu)*Y, lambda/mu);
		Y = Y + mu*(X-L-S);
		count = count + 1;
	  end
	end

	function out = SVT(X, tau)
	  [U, E, V] = svd(X, 'econ');
	  out = U*shrink(E, tau)*V';
	end

	function out = shrink(X, tau)
	  out = sign(X).*max(abs(X) - tau, 0);
	end
*/