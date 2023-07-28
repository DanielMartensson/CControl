/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

static void center_data(float X[], float mu[], size_t row, size_t column);
static void compute_components(float X[], float W[], size_t components, size_t row, size_t column);

/*
 * Principal Component Analysis (PCA)
 * This compute the PCA of X and return components W
 * X[m*n]
 * W[n*components]
 * P[m*components]
 * mu[n]
 */
void pca(float X[], float W[], float P[], float mu[], size_t components, size_t row, size_t column) {
	/* Copy the data X -> Z */
	float* Z = (float*)malloc(row * column * sizeof(float));
	memcpy(Z, X, row * column * sizeof(float));

	/* Average and center data Z = Z - mean(Z) */
	center_data(Z, mu, row, column);

	/* Do covariance Y = cov(Z) */
	float* Y = (float*)malloc(column * column * sizeof(float));
	covm(Z, Y, row, column);

	/* Get components [U, S, V] = svd(Y) */
	compute_components(Y, W, components, column, column);

	/* Project P = Z*W */
	mul(Z, W, P, row, column, components);

	/* Free */
	free(Y);
	free(Z);
}

static void center_data(float X[], float mu[], size_t row, size_t column) {
	size_t i, j;
	float* X0 = X;
	float average;
	/* Column-Major */
	tran(X, row, column);
	for (i = 0; i < column; i++) {
		/* Average data mu[i] = mean(X) */
		average = mean(X, row);

		/* Center the data X = X - mu */
		for (j = 0; j < row; j++) {
			X[j] -= average;
		}

		/* Save the average */
		mu[i] = average;

		/* Jump to next row */
		X += row;
	}
	X = X0;

	/* Turn it back to Row-Major */
	tran(X, column, row);
}

static void compute_components(float X[], float W[], size_t components, size_t row, size_t column) {
	/* Compute [U, S, V] = svd(A) */
	float* U = (float*)malloc(row * column * sizeof(float));
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));
	float* V0 = V;
	svd_jacobi_one_sided(X, column, U, S, V);

	/* Get the components from V */
	size_t i, bytes_shift = components * sizeof(float);
	for (i = 0; i < column; i++) {
		memcpy(W, V, bytes_shift); /* Move data from V to W, then shift the array position */
		W += components;
		V += column;
	}

	/* Reset */
	V = V0;

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
	  Z = X - mu;

	  % Create the covariance
	  Y = cov(Z);

	  % PCA analysis
	  [~, ~, V] = svd(Y, 0);

	  % Projection
	  W = V(:, 1:c);
	  P = Z*W;
	end
*/