/*
 * pca.c
 *
 *  Created on: 24 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "statistics.h"

static void compute_components(float X[], float W[], size_t c, size_t row, size_t column);

/* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "../LinearAlgebra/CLapack/clapack.h"
#endif

/*
 * Principal Component Analysis (PCA)
 * This compute the PCA of X and return components W and projection P and average mu
 * X[m*n]
 * W[m*c]
 * P[c*n]
 * mu[n]
 */
void pca(const float X[], float W[], float P[], float mu[], const size_t c, const size_t row, const size_t column) {
	/* Copy the data X -> Y */
	float* Y = (float*)malloc(row * column * sizeof(float));
	memcpy(Y, X, row * column * sizeof(float));

	/* Average and center data Y = Y - mean(Y) */
	center(Y, mu, row, column);

	/* Get components [U, S, V] = svd(Y), where U = W */
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
	/* Allocate */
	float* U = (float*)malloc(row * row * sizeof(float));
	float* U0 = U;
	float* S = (float*)malloc(column * sizeof(float));
	float* V = (float*)malloc(column * column * sizeof(float));

#ifdef CLAPACK_USED
	/* Settings for computing only U matrix from SVD */
	integer m = row, n = column, lda = row, ldu = row, ldvt = column, lwork, info;
	real wkopt;
	real* work = NULL;

	/* Important to take transpose */
	float* Xcopy = (float*)malloc(row * column * sizeof(float));
	size_t i, j;
	float* Xcopy0 = Xcopy;
	for (i = 0; i < row; i++) {
		Xcopy = Xcopy0;
		Xcopy += i;
		for (j = 0; j < column; j++) {
			Xcopy[0] = X[j];
			Xcopy += row;
		}
		X += column;
	}
	Xcopy = Xcopy0;

	/* Allocate memory for U matrix only */
	lwork = -1;
	sgesvd_("S", "N", &m, &n, Xcopy, &lda, S, U, &ldu, V, &ldvt, &wkopt, &lwork, &info);
	lwork = (integer)wkopt;
	work = (real*)malloc(lwork * sizeof(real));

	/* Compute U matrix only */
	sgesvd_("S", "N", &m, &n, Xcopy, &lda, S, U, &ldu, V, &ldvt, work, &lwork, &info);
	/* Fill W with transpose */
	for (i = 0; i < row; i++) {
		U = U0;
		U += i;
		for (j = 0; j < c; j++) {
			W[j] = U[0];
			U += row;
		}
		W += c;
	}
	
	/* Free */
	free(work);
	free(Xcopy);
#elif defined(MKL_LAPACK_USED)
	float* superb = (float*)malloc((min(row, column) - 1)*sizeof(float));
	float* Y = (float*)malloc(row * column * sizeof(float));
	memcpy(Y, X, row * column * sizeof(float));
	LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'S', 'N', row, column, Y, column, S, U, row, NULL, column, superb);
	free(superb);
	free(Y);

	/* Get the components from U */
	size_t i, bytes_shift = c * sizeof(float);
	for (i = 0; i < row; i++) {
		memcpy(W, U, bytes_shift); /* Move data from U to W, then shift the array position */
		W += c;
		U += row;
	}
#else
	/* Allocate 
	float* U = (float*)malloc(row * column * sizeof(float));
	float* U0 = U;*/

	/* Compute */
	svd(X, row, column, U, S, V);

	/* Get the components from U */
	size_t i, bytes_shift = c * sizeof(float);
	for (i = 0; i < row; i++) {
		memcpy(W, U, bytes_shift); /* Move data from U to W, then shift the array position */
		W += c;
		U += column;
	}
#endif

	/* Reset */
	U = U0;

	/* Free */
	free(U);
	free(S);
	free(V);
}

/* GNU Octave code:
	% Principal Component Analysis
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
	  mu = mean(X); %clusterfilter(X)

	  % Center data
	  Y = X - mu;

	  % PCA analysis
	  abort = input(sprintf('The size of the matrix is %ix%i. Do you want to apply PCA onto it? 1 = Yes, 0 = No: ', size(Y)));
	  if(abort == 1)
		[U, ~, ~] = svdecon(Y);
	  else
		error('Aborted');
	  end

	  % Projection
	  W = U(:, 1:c);
	  P = W'*Y;
	end

	function [U,S,V] = svdecon(X)
	  % Input:
	  % X : m x n matrix
	  %
	  % Output:
	  % X = U*S*V'
	  %
	  % Description:
	  % Does equivalent to svd(X,'econ') but faster
	  %
	  % Vipin Vijayan (2014)
	  %X = bsxfun(@minus,X,mean(X,2));
	  [m,n] = size(X);
	  if  m <= n
		  C = X*X';
		  [U,D] = eig(C);
		  clear C;

		  [d,ix] = sort(abs(diag(D)),'descend');
		  U = U(:,ix);

		  if nargout > 2
			  V = X'*U;
			  s = sqrt(d);
			  V = bsxfun(@(x,c)x./c, V, s');
			  S = diag(s);
		  end
	  else
		  C = X'*X;
		  [V,D] = eig(C);
		  clear C;

		  [d,ix] = sort(abs(diag(D)),'descend');
		  V = V(:,ix);

		  U = X*V; % convert evecs from X'*X to X*X'. the evals are the same.
		  %s = sqrt(sum(U.^2,1))';
		  s = sqrt(d);
		  U = bsxfun(@(x,c)x./c, U, s');
		  S = diag(s);
	  end
	end
*/