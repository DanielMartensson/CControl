/*
 * svd.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Regular SVD routines */
bool svd_golub_reinsch(float A[], size_t row, size_t column, float U[], float S[], float V[]);
bool svd_jacobi_one_sided(float A[], size_t row, float U[], float S[], float V[]);

/* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * Compute Singular Value decomposition
 * A[m*n]
 * U[m*n]
 * S[n]
 * V[n*n]
 * Return true = Success
 * Return false = fail
 */
bool svd(const float A[], const size_t row, const size_t column, float U[], float S[], float V[]) {
	/* Check if matrix is symmetric */
	bool symmetric = issymmetric(A, row, row);

#ifdef CLAPACK_USED
	integer info = -1;
	if (symmetric) {
		/* Settings */
		integer n = column, lda = row, lwork, liwork;
		integer iwkopt;
		integer* iwork = NULL;
		real wkopt;
		real* work = NULL;

		/* Copy over A -> a */
		real* a = (real*)malloc(lda * n * sizeof(real));
		memcpy(a, A, lda * n * sizeof(real));

		/* Allocate memory */
		lwork = -1;
		liwork = -1;
		ssyevd_("V", "U", &n, a, &lda, S, &wkopt, &lwork, &iwkopt, &liwork, &info);
		lwork = (integer)wkopt + 1; /* +1 will fix the large matrix issue */
		work = (real*)malloc(lwork * sizeof(real));
		liwork = iwkopt;
		iwork = (integer*)malloc(liwork * sizeof(integer));

		/* Compute */
		ssyevd_("V", "U", &n, a, &lda, S, work, &lwork, iwork, &liwork, &info);

		/* Swap */
		size_t i;
		float v;
		size_t start = 0;
		size_t end = row - 1;
		while (start < end) {
			v = S[start];
			S[start] = S[end];
			S[end] = v;
			start++;
			end--;
		}

		/* Move over to U via transpose */
		size_t j, s = 0;
		float* U0 = U;
		for (i = 0; i < row; i++) {
			U = U0;
			U += row - i - 1;
			for (j = 0; j < row; j++) {
				U[0] = a[s++];
				U += row;
			}
		}
		U = U0;

		/* Move over to V */
		memcpy(V, U, row * row * sizeof(float));

		/* Free */
		free(a);
		free(work);
		free(iwork);
	}
	else {
		/* Settings */
		integer m = row, n = column, lda = row, ldu = row, ldvt = column, lwork;
		real wkopt;
		real* work = NULL;
		real* u = (real*)malloc(m * n * sizeof(real));
		real* iwork = (real*)malloc(8 * vmin(m, n) * sizeof(real));

		/* Important to take transpose */
		float* Acopy = (float*)malloc(row * column * sizeof(float));
		size_t i, j;
		float* Acopy0 = Acopy;
		for (i = 0; i < row; i++) {
			Acopy = Acopy0;
			Acopy += i;
			for (j = 0; j < column; j++) {
				Acopy[0] = A[j];
				Acopy += row;
			}
			A += column;
		}
		Acopy = Acopy0;

		/* Allocate memory */
		lwork = -1;
		sgesdd_("S", &m, &n, Acopy, &lda, S, u, &ldu, V, &ldvt, &wkopt, &lwork, iwork, &info);
		lwork = (integer)wkopt;
		work = (real*)malloc(lwork * sizeof(real));

		/* Compute */
		sgesdd_("S", &m, &n, Acopy, &lda, S, u, &ldu, V, &ldvt, work, &lwork, iwork, &info);

		/* Fill U with transpose */
		real* u0 = u;
		for (i = 0; i < row; i++) {
			u = u0;
			u += i;
			for (j = 0; j < column; j++) {
				U[j] = u[0];
				u += row;
			}
			U += column;
		}

		/* Reset */
		u = u0;

		/* Free */
		free(work);
		free(u);
		free(Acopy);
		free(iwork);
	}

	/* Return status */
	return info == 0;
#elif defined(MKL_LAPACK_USED)
	if (symmetric) {
		/* Copy over */
		memcpy(U, A, row * column * sizeof(float));
		
		/* Do SVD */
		int status = LAPACKE_ssyevd(MKL_ROW_MAJOR, 'V', 'U', row, U, row, S);

		/* Sort S */
		size_t* index = (size_t*)malloc(column * sizeof(size_t));
		sort(S, index, 1, column, SORT_MODE_COLUMN_DIRECTION_DESCEND);
		
		/* Sort V */
		size_t i, j;
		float* U0 = U;
		float* V0 = V;
		for (i = 0; i < row; i++) {
			for (j = 0; j < column; j++) {
				V[j] = U[index[j]];
			}
			V += column;
			U += column;
		}
		V = V0;
		U = U0;

		/* Free index */
		free(index);

		/* Copy over */
		memcpy(U, V, row * column * sizeof(float));
		return status == 0;
	}
	else {
		/* Copy */
		float* Acopy = (float*)malloc(row * column * sizeof(float));
		memcpy(Acopy, A, row * column * sizeof(float));
		int m = row;
		int n = column;
		float* u = (float*)malloc(m * m * sizeof(float));
		int status = LAPACKE_sgesdd(LAPACK_ROW_MAJOR, 'S', m, n, Acopy, n, S, u, m, V, n);
		
		/* Get the components from U */
		float* u0 = u;
		size_t i, bytes_shift = column * sizeof(float);
		for (i = 0; i < row; i++) {
			memcpy(U, u, bytes_shift); /* Move data from u to U, then shift the array position */
			U += column;
			u += row;
		}

		/* Reset */
		u = u0;

		/* Free */
		free(Acopy);
		free(u);
		return status == 0;
	}
#else
	if (symmetric) {
		return svd_jacobi_one_sided(A, row, U, S, V);
	}
	else {
		return svd_golub_reinsch(A, row, column, U, S, V);
	}
#endif
}

/*
  * This is Singular Value Decomposition X = USV^T
  * This uses Jacobi rotation method.
  * http://www.netlib.org/lapack/lawnspdf/lawn15.pdf
  * Use this SVD method if you have a square general matrix X.
  * A [m*n]
  * U [m*m]
  * S [n]
  * V [n*n]
  * n == m
  * Return true = Success
  * Return false = fail
  */
bool svd_jacobi_one_sided(float A[], size_t row, float U[], float S[], float V[]) {
	/* Copy over A to Acopy */
	float* Acopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Acopy, A, row * row * sizeof(float));

	/* i and j are the indices of the point we've chosen to zero out */
	float al, b, c, l, t, cs, sn, tmp, sign, error;
	size_t i, j, p, k;

	/* Save address */
	float* A0 = Acopy;
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
				Acopy = A0;
				for (k = 0; k < row; k++) {
					al += Acopy[i] * Acopy[i];
					b += Acopy[j] * Acopy[j];
					c += Acopy[j] * Acopy[i];
					Acopy += row;
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
				Acopy = A0;
				for (k = 0; k < row; k++) {
					tmp = Acopy[i];
					Acopy[i] = cs * tmp - sn * Acopy[j];
					Acopy[j] = sn * tmp + cs * Acopy[j];
					Acopy += row;
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
		Acopy = A0;
		free(Acopy);
		return false;
	}

	/* Find the singular values by adding up squares of each column, then taking square root of each column */
	for (j = 0; j < row; j++) {
		Acopy = A0;
		for (i = 0; i < row; i++) {
			S[j] += Acopy[j] * Acopy[j];
			Acopy += row;
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
				Acopy = A0;
				for (i = 0; i < row; i++) {
					tmp = V[j];
					V[j] = V[j + 1];
					V[j + 1] = tmp;
					tmp = Acopy[j];
					Acopy[j] = Acopy[j + 1];
					Acopy[j + 1] = tmp;
					Acopy += row;
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
	Acopy = A0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < row; j++) {
			Acopy[j] /= S[j];
			/* A[row*i + j] = A[row*i + j] / S[j]; */
		}
		Acopy += row;
	}

	/* Set U to A, since we have been making modifications to A */
	Acopy = A0;
	U = U0;
	memcpy(U, Acopy, row * row * sizeof(float));

	/* Free */
	free(Acopy);

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

 /* Private functions */
static void Householders_Reduction_to_Bidiagonal_Form(float* A, size_t nrows, size_t ncols, float* U, float* V, float* diagonal, float* superdiagonal);
static bool Givens_Reduction_to_Diagonal_Form(size_t nrows, size_t ncols, float* U, float* V, float* diagonal, float* superdiagonal);
static void Sort_by_Decreasing_Singular_Values(size_t nrows, size_t ncols, float* singular_value, float* U, float* V);

/*
 *
 *     This is Singular Value Decomposition A = USV^T.
 *     This routine decomposes an m x n matrix A, with m >= n, into a product
 * 	   of the three matrices U, D, and V', i.e. A = UDV', where U is an m x n
 *     matrix whose columns are orthogonal, D is a n x n diagonal matrix, and
 *     V is an n x n orthogonal matrix.  V' denotes the transpose of V.  If
 *     m < n, then the procedure may be used for the matrix A'.  The singular
 *     values of A are the diagonal elements of the diagonal matrix D and
 *     correspond to the positive square roots of the eigenvalues of the
 *     matrix A'A.
 *
 *     This procedure programmed here is based on the method of Golub and
 *     Reinsch as given on pages 134 - 151 of the "Handbook for Automatic
 *     Computation vol II - Linear Algebra" edited by Wilkinson and Reinsch
 *     and published by Springer-Verlag, 1971.
 *
 *     The Golub and Reinsch's method for decomposing the matrix A into the
 *     product U, D, and V' is performed in three stages:
 *       Stage 1:  Decompose A into the product of three matrices U1, B, V1'
 *         A = U1 B V1' where B is a bidiagonal matrix, and U1, and V1 are a
 *         product of Householder transformations.
 *       Stage 2:  Use Given' transformations to reduce the bidiagonal matrix
 *         B into the product of the three matrices U2, D, V2'.  The singular
 *         value decomposition is then UDV'where U = U2 U1 and V' = V1' V2'.
 *       Stage 3:  Sort the matrix D in decreasing order of the singular
 *         values and interchange the columns of both U and V to reflect any
 *         change in the order of the singular values.
 * A [m*n]
 * U [m*n]
 * S [n]
 * V [n*n]
 * m = row
 * n = column
 * If m < n, then this procedure is for A'
 * Return true = Success.
 * Return false = Fail.
 */
bool svd_golub_reinsch(float A[], size_t row, size_t column, float U[], float S[], float V[]) {
	float* dummy_array = (float*)malloc(column * sizeof(float));

	Householders_Reduction_to_Bidiagonal_Form(A, row, column, U, V, S, dummy_array);

	bool ok = true;
	if (!Givens_Reduction_to_Diagonal_Form(row, column, U, V, S, dummy_array)) {
		ok = false; /* Fail */
	}

	Sort_by_Decreasing_Singular_Values(row, column, S, U, V);

	free(dummy_array);

	return ok; /* Solved */
}

static void Householders_Reduction_to_Bidiagonal_Form(float* A, size_t nrows, size_t ncols, float* U, float* V, float* diagonal, float* superdiagonal) {
	int32_t i, j, k, ip1;
	float s, s2, si, scale;
	float* pu, * pui, * pv, * pvi;
	float half_norm_squared;

	/* Copy A to U */
	memcpy(U, A, sizeof(float) * nrows * ncols);

	diagonal[0] = 0.0f;
	s = 0.0f;
	scale = 0.0f;
	for (i = 0, pui = U, ip1 = 1; i < ncols; pui += ncols, i++, ip1++) {
		superdiagonal[i] = scale * s;
		/*
			   Perform Householder transform on columns.

			   Calculate the normed squared of the i-th column vector starting at
			   row i.
		*/
		for (j = i, pu = pui, scale = 0.0f; j < nrows; j++, pu += ncols) {
			scale += fabsf(pu[i]);
		}

		if (scale > 0.0f) {
			for (j = i, pu = pui, s2 = 0.0f; j < nrows; j++, pu += ncols) {
				pu[i] /= scale;
				s2 += pu[i] * pu[i];
			}
			/* Chose sign of s which maximizes the norm */
			s = (pu[i] < 0.0f) ? sqrtf(s2) : -sqrtf(s2);

			/* Calculate -2/u'u */
			half_norm_squared = pui[i] * s - s2;

			/* Transform remaining columns by the Householder transform. */
			pui[i] -= s;

			for (j = ip1; j < ncols; j++) {
				for (k = i, si = 0.0f, pu = pui; k < nrows; k++, pu += ncols) {
					si += pu[i] * pu[j];
				}
				si /= half_norm_squared;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols) {
					pu[j] += si * pu[i];
				}
			}
		}
		for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
			pu[i] *= scale;
		}
		diagonal[i] = s * scale;
		/* Perform Householder transform on rows. */
		/* Calculate the normed squared of the i-th row vector starting at column i. */
		s = 0.0f;
		scale = 0.0f;
		if (i >= nrows || i == (ncols - 1)) {
			continue;
		}
		for (j = ip1; j < ncols; j++) {
			scale += fabsf(pui[j]);
		}
		if (scale > 0.0) {
			for (j = ip1, s2 = 0.0f; j < ncols; j++) {
				pui[j] /= scale;
				s2 += pui[j] * pui[j];
			}
			s = (pui[ip1] < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			/* Calculate -2/u'u */
			half_norm_squared = pui[ip1] * s - s2;
			/* Transform the rows by the Householder transform. */

			pui[ip1] -= s;
			for (k = ip1; k < ncols; k++) {
				superdiagonal[k] = pui[k] / half_norm_squared;
			}
			if (i < (nrows - 1)) {
				for (j = ip1, pu = pui + ncols; j < nrows; j++, pu += ncols) {
					for (k = ip1, si = 0.0f; k < ncols; k++) {
						si += pui[k] * pu[k];
					}
					for (k = ip1; k < ncols; k++) {
						pu[k] += si * superdiagonal[k];
					}
				}
			}
			for (k = ip1; k < ncols; k++) {
				pui[k] *= scale;
			}
		}
	}

	/* Update V */
	pui = U + ncols * (ncols - 2);
	pvi = V + ncols * (ncols - 1);
	*(pvi + ncols - 1) = 1.0;
	s = superdiagonal[ncols - 1];
	pvi -= ncols;
	for (i = ncols - 2, ip1 = ncols - 1; i >= 0; i--, pui -= ncols, pvi -= ncols, ip1--) {
		if (fabsf(s) > MIN_VALUE) {
			pv = pvi + ncols;
			for (j = ip1; j < ncols; j++, pv += ncols) {
				pv[i] = (pui[j] / pui[ip1]) / s;
			}
			for (j = ip1; j < ncols; j++) {
				si = 0.0f;
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols) {
					si += pui[k] * pv[j];
				}
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols) {
					pv[j] += si * pv[i];
				}
			}
		}
		pv = pvi + ncols;
		for (j = ip1; j < ncols; j++, pv += ncols) {
			pvi[j] = 0.0f;
			pv[i] = 0.0f;
		}
		pvi[i] = 1.0f;
		s = superdiagonal[i];
	}

	/* Update U */

	pui = U + ncols * (ncols - 1);
	for (i = ncols - 1, ip1 = ncols; i >= 0; ip1 = i, i--, pui -= ncols) {
		s = diagonal[i];
		for (j = ip1; j < ncols; j++) {
			pui[j] = 0.0f;
		}
		if (fabsf(s) > MIN_VALUE) {
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				pu = pui + ncols;
				for (k = ip1; k < nrows; k++, pu += ncols) {
					si += pu[i] * pu[j];
				}
				si = (si / pui[i]) / s;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols) {
					pu[j] += si * pu[i];
				}
			}
			for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
				pu[i] /= s;
			}
		}
		else
			for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
				pu[i] = 0.0f;
			}
		pui[i] += 1.0f;
	}
}

static bool Givens_Reduction_to_Diagonal_Form(size_t nrows, size_t ncols, float* U, float* V, float* diagonal, float* superdiagonal) {

	float epsilon;
	float c, s;
	float f, g, h;
	float x, y, z;
	float* pu, * pv;
	int32_t i, j, k, m;
	int32_t rotation_test;
	int32_t iteration_count;

	for (i = 0, x = 0.0f; i < ncols; i++) {
		y = fabsf(diagonal[i]) + fabsf(superdiagonal[i]);
		if (x < y) {
			x = y;
		}
	}
	epsilon = x * FLT_EPSILON;
	for (k = ncols - 1; k >= 0; k--) {
		iteration_count = 0;
		while (1) {
			rotation_test = 1;
			for (m = k; m >= 0; m--) {
				if (fabsf(superdiagonal[m]) <= epsilon) {
					rotation_test = 0;
					break;
				}
				if (fabsf(diagonal[m - 1]) <= epsilon) {
					break;
				}
			}
			if (rotation_test) {
				c = 0.0f;
				s = 1.0f;
				for (i = m; i <= k; i++) {
					f = s * superdiagonal[i];
					superdiagonal[i] *= c;
					if (fabsf(f) <= epsilon) {
						break;
					}
					g = diagonal[i];
					h = sqrtf(f * f + g * g);
					diagonal[i] = h;
					c = g / h;
					s = -f / h;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = pu[m - 1];
						z = pu[i];
						pu[m - 1] = y * c + z * s;
						pu[i] = -y * s + z * c;
					}
				}
			}
			z = diagonal[k];
			if (m == k) {
				if (z < 0.0f) {
					diagonal[k] = -z;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols) {
						pv[k] = -pv[k];
					}
				}
				break;
			}
			else {
				if (iteration_count >= MAX_ITERATIONS) {
					return false;
				}
				iteration_count++;
				x = diagonal[m];
				y = diagonal[k - 1];
				g = superdiagonal[k - 1];
				h = superdiagonal[k];
				f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0f * h * y);
				g = sqrtf(f * f + 1.0f);
				if (f < 0.0f) {
					g = -g;
				}
				f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
				/* Next QR Transformtion */
				c = 1.0f;
				s = 1.0f;
				for (i = m + 1; i <= k; i++) {
					g = superdiagonal[i];
					y = diagonal[i];
					h = s * g;
					g *= c;
					z = sqrtf(f * f + h * h);
					superdiagonal[i - 1] = z;
					c = f / z;
					s = h / z;
					f = x * c + g * s;
					g = -x * s + g * c;
					h = y * s;
					y *= c;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols) {
						x = pv[i - 1];
						z = pv[i];
						pv[i - 1] = x * c + z * s;
						pv[i] = -x * s + z * c;
					}
					z = sqrtf(f * f + h * h);
					diagonal[i - 1] = z;
					if (z > MIN_VALUE) {
						c = f / z;
						s = h / z;
					}
					f = c * g + s * y;
					x = -s * g + c * y;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = pu[i - 1];
						z = pu[i];
						pu[i - 1] = c * y + s * z;
						pu[i] = -s * y + c * z;
					}
				}
				superdiagonal[m] = 0.0f;
				superdiagonal[k] = f;
				diagonal[k] = x;
			}
		}
	}
	return true;
}

static void Sort_by_Decreasing_Singular_Values(size_t nrows, size_t ncols, float* singular_values, float* U, float* V) {
	size_t i, j, max_index;
	float temp;
	float* p1, * p2;

	for (i = 0; i < ncols - 1; i++) {
		max_index = i;
		for (j = i + 1; j < ncols; j++) {
			if (singular_values[j] > singular_values[max_index]) {
				max_index = j;
			}
		}
		if (max_index == i) {
			continue;
		}
		temp = singular_values[i];
		singular_values[i] = singular_values[max_index];
		singular_values[max_index] = temp;
		p1 = U + max_index;
		p2 = U + i;
		for (j = 0; j < nrows; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
		p1 = V + max_index;
		p2 = V + i;
		for (j = 0; j < ncols; j++, p1 += ncols, p2 += ncols) {
			temp = *p1;
			*p1 = *p2;
			*p2 = temp;
		}
	}
}

/*
 * GNU Octave code:
 *  >> A = [3 4 5; 3 5 6; 6 7 8; 3 5 6];
	>> [u, s, v] = svd(A)
	u =

	  -3.8485e-01   4.1233e-02   9.2206e-01  -5.6889e-16
	  -4.5421e-01   4.9880e-01  -2.1189e-01  -7.0711e-01
	  -6.6278e-01  -7.0760e-01  -2.4499e-01   1.8751e-16
	  -4.5421e-01   4.9880e-01  -2.1189e-01   7.0711e-01

	s =

	Diagonal Matrix

	   18.36763          0          0
			  0    1.26360          0
			  0          0    0.18280
			  0          0          0

	v =

	  -0.42774  -0.89359   0.13617
	  -0.58369   0.15803  -0.79645
	  -0.69018   0.42015   0.58917

	>>
 */