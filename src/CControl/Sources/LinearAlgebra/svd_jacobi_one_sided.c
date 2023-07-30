/*
 * svd_jacobi_one_sided.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

 /*
  * This is Singular Value Decomposition X = USV^T
  * This uses Jacobi rotation method.
  * http://www.netlib.org/lapack/lawnspdf/lawn15.pdf
  * Use this SVD method if you have a square matrix X.
  * X [m*n]
  * U [m*m]
  * S [n]
  * V [n*n]
  * n == m
  * Return true = Success
  * Return false = fail
  */
bool svd_jacobi_one_sided(float X[], size_t row, float U[], float S[], float V[]) {
	/* Copy over X to A */
	float* A = (float*)malloc(row * row * sizeof(float));
	memcpy(A, X, row * row * sizeof(float));

	/* i and j are the indices of the point we've chosen to zero out */
	float al, b, c, l, t, cs, sn, tmp, sign, error;
	size_t i, j, p, k;

	/* Save address */
	float* A0 = A;
	float* U0 = U;
	float* V0 = V;

	/* Create the identity matrix */
	memset(U, 0, row * row * sizeof(float));
	memset(V, 0, row * row * sizeof(float));
	memset(S, 0, row * sizeof(float));
	for (i = 0; i < row; i++) {
		U[i] = 1.0f;
		V[i] = 1.0f;
		U += row;
		V += row;
		/* U[row*i + i] = 1.0f; */
		/* V[row*i + i] = 1.0f; */
	}

	/* Apply max_iterations times. That should be good enough */
	bool exit = false;
	for (p = 0; p < MAX_ITERATIONS; p++) {
		/* For all pairs i < j */
		error = 0.0f;
		for (i = 0; i < row; i++) {
			for (j = i + 1; j < row; j++) {
				al = b = c = l = t = cs = sn = tmp = sign = 0.0f;
				/* Find the 2x2 submatrix */
				A = A0;
				for (k = 0; k < row; k++) {
					al += A[i] * A[i];
					b += A[j] * A[j];
					c += A[j] * A[i];
					A += row;
					/* al += A[row*k + i] * A[row*k + i]; */
					/* b += A[row*k + j] * A[row*k + j]; */
					/* c += A[row*k + i] * A[row*k + j]; */
				}

				/* Compute the error */
				error = vmax(error, fabsf(c) / sqrtf(al * b));
				if (error < MIN_VALUE) {
					exit = true;
					break;
				}

				/* Compute Jacobi rotation */
				l = (b - al) / (2.0f * c);
				sign = 1.0f;
				if (l < 0.0f) {
					sign = -1.0f;
				}
				t = sign / ((sign * l) + sqrtf(1.0f + l * l));
				cs = 1.0f / sqrtf(1.0f + t * t);
				sn = cs * t;

				/* Change columns i and j only */
				A = A0;
				for (k = 0; k < row; k++) {
					tmp = A[i];
					A[i] = cs * tmp - sn * A[j];
					A[j] = sn * tmp + cs * A[j];
					A += row;
					/* tmp = A[row*k + i]; */
					/* A[row*k + i] = cs * tmp - sn * A[row*k + j]; */
					/* A[row*k + j] = sn * tmp + cs * A[row*k + j]; */
				}

				/* Update the right singular vectors */
				V = V0;
				for (k = 0; k < row; k++) {
					tmp = V[i];
					V[i] = cs * tmp - sn * V[j];
					V[j] = sn * tmp + cs * V[j];
					V += row;
					/* tmp = *(V + row*k + i); */
					/* V[row*k + i] = cs * tmp - sn * V[row*k + j]; */
					/* V[row*k + j] = sn * tmp + cs * V[row*k + j]; */
				}
			}
			if (exit) {
				break;
			}
		}
		if (exit) {
			break;
		}
	}

	/* If something wrong happens */
	if (p == MAX_ITERATIONS) {
		A = A0;
		free(A);
		return false;
	}

	/* Find the singular values by adding up squares of each column, then taking square root of each column */
	for (j = 0; j < row; j++) {
		A = A0;
		for (i = 0; i < row; i++) {
			S[j] += A[j] * A[j];
			A += row;
			/* S[j] += A[row*i + j] * A[row*i + j]; */
		}
		tmp = S[j];
		S[j] = sqrtf(tmp);
	}

	/* Sort the singular values largest to smallest, and the right matrix accordingly */
	for (p = 0; p < (row - 1); p++) {
		for (j = 0; j < row - p - 1; j++) {
			if (S[j] < S[j + 1]) {
				tmp = S[j];
				S[j] = S[j + 1];
				S[j + 1] = tmp;

				/* Rearrange columns of u and v accordingly */
				V = V0;
				A = A0;
				for (i = 0; i < row; i++) {
					tmp = V[j];
					V[j] = V[j + 1];
					V[j + 1] = tmp;
					tmp = A[j];
					A[j] = A[j + 1];
					A[j + 1] = tmp;
					A += row;
					V += row;
					/* tmp = V[row*i + j]; */
					/* V[row*i + j] = V[row*i + j + 1]; */
					/* V[row*i + j + 1] = tmp; */
					/* tmp = A[row*i + j]; */
					/* A[row*i + j] = A[row*i + j + 1]; */
					/* A[row*i + j + 1] = tmp; */
				}
			}
		}
	}

	/* A is A*V, so in order to get U, we divide by S in each column */
	A = A0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			A[j] /= S[j];
			/* A[row*i + j] = A[row*i + j] / S[j]; */
		}
		A += row;
	}

	/* Set U to A, since we have been making modifications to A */
	A = A0;
	U = U0;
	memcpy(U, A, row * row * sizeof(float));

	/* Free */
	free(A);

	/* Return OK */
	return true;
}

/*
 * GNU Octave code:
 *  function [U,S,V] = svd(A)
	% Jacobi One Sided SVD program
	%
	% A simple program that demonstrates how to use the jacobi method to obtain SVD
	% A may be rectangular and complex.
	%
	% usage: [U,S,V]= svd(A)
	%     or      S = svd(A)
	%
	% with A = U*S*V' , S>=0 , U'*U = Iu  , and V'*V = Iv
	error = 1;
	n = size(A, 2);
	V = eye(n); S = eye(n);
	while error > 1e-6
		error = 0;
		for i = 1:n-1
		for j = i+1:n
			x = A(:,i);
			y = A(:,j);
			a = x'*x;
			b = y'*y;
			c = x'*y;

			error = max(error,abs(c)/sqrt(a*b))

			%Jacobi rotation
			e = (b - a)/(2*c);
			t = sign(e)/(abs(e) + sqrt(1 + e^2));
			cs = 1/sqrt(1+t^2);
			sn = cs*t;

			% update G
			x = A(:,i);
			A(:,i) = cs*x - sn*A(:,j);
			A(:,j) = sn*x + cs*A(:,j);

			% update V
			x = V(:,i);
			V(:,i) = cs*x - sn*V(:,j);
			V(:,j) = sn*x + cs*V(:,j);

		end
		end
	end

	U = A;
	for i = 1:n
		S(i,i) = norm(A(:,i));
		U(:,i) = A(:,i)/S(i,i);
	end
 */
