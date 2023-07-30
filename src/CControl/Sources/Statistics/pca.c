/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void compute_components(float X[], float W[], size_t c, size_t row, size_t column);

/*
 * Principal Component Analysis (PCA)
 * This compute the PCA of X and return components W
 * X[m*n]
 * W[m*c]
 * P[c*n]
 * mu[n]
 */
void pca(float X[], float W[], float P[], float mu[], size_t c, size_t row, size_t column) {
	/* Copy the data X -> Y */
	float* Y = (float*)malloc(row * column * sizeof(float));
	memcpy(Y, X, row * column * sizeof(float));

	/* Average and center data Y = Y - mean(Y) */
	center(Y, mu, row, column);

	/* Get components [U, S, V] = svd(Y) */
	compute_components(Y, W, c, row, column);

	/* Transpose W */
	float* Wt = (float*)malloc(row * c * sizeof(float));
	memcpy(Wt, W, row * c * sizeof(float));
	tran(Wt, row, c);

	/* Project P = W'*Y */
	mul(Wt, Y, P, c, row, column);

	/* Free */
	free(Y);
	free(Wt);
}

static void compute_components(float X[], float W[], size_t c, size_t row, size_t column) {
	/* Compute [U, S, V] = svd(A) */
	float* U = (float*)malloc(row * column * sizeof(float));
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	float* U0 = U;
	if (row == column) {
		svd_jacobi_one_sided(X, row, U, S, V);
	}
	else {
		svd_golub_reinsch(X, row, column, U, S, V);
	}

	/* Get the components from V */
	size_t i, bytes_shift = c * sizeof(float);
	for (i = 0; i < row; i++) {
		memcpy(W, U, bytes_shift); /* Move data from U to W, then shift the array position */
		W += c;
		U += column;
	}

	/* Reset */
	U = U0;

	/* Free */
	free(U);
	free(S);
	free(V);
}

/* GNU Octave code:
	% Principal Component Analysis with cluster filtering
	% Input: X(Data), c(Amount of components)
	% Output: Projected matrix P, Project matrix W, mu(Average vector of X)
	% Example 1: [P, W] = mi.pca(X, c);
	% Example 2: [P, W, mu] = mi.pca(X, c);
	% Author: Daniel Mårtensson, 2023 April

	function [P, W, mu] = pca(varargin)
	  % Check if there is any input
	  if(isempty(varargin))
		error('Missing inputs')
	  end

	  % Get impulse response
	  if(length(varargin) >= 1)
		X = varargin{1};
	  else
		error('Missing data X')
	  end

	  % Get the amount of components
	  if(length(varargin) >= 2)
		c = varargin{2};
	  else
		error('Missing amount of components');
	  end

	  % Average
	  mu = mean(X); %cluster_filter(X)

	  % Center data
	  Y = X - mu;

	  % PCA analysis
	  [U, ~, ~] = svd(Y, 0);

	  % Projection
	  W = U(:, 1:c);
	  P = W'*Y;
	end
*/