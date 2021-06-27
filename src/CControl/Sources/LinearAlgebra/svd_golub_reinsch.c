/*
 * svd_golub_reinsch.c
 *
 *  Created on: 1 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

// Private functions
static void Householders_Reduction_to_Bidiagonal_Form(float *A, uint16_t nrows, uint16_t ncols, float *U, float *V, float *diagonal, float *superdiagonal);
static int Givens_Reduction_to_Diagonal_Form(uint16_t nrows, uint16_t ncols, float *U, float *V, float *diagonal, float *superdiagonal);
static void Sort_by_Decreasing_Singular_Values(uint16_t nrows, uint16_t ncols, float *singular_value, float *U, float *V);


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
 * Return 1 = Success.
 * Return 0 = Fail.
 */
uint8_t svd_golub_reinsch(float A[], uint16_t row, uint16_t column, float U[], float S[], float V[]) {
	float dummy_array[column];

	Householders_Reduction_to_Bidiagonal_Form(A, row, column, U, V, S, dummy_array);

	if (Givens_Reduction_to_Diagonal_Form(row, column, U, V, S, dummy_array) < 0)
		return 0; // Fail

	Sort_by_Decreasing_Singular_Values(row, column, S, U, V);

	return 1; // Solved
}

static void Householders_Reduction_to_Bidiagonal_Form(float *A, uint16_t nrows, uint16_t ncols, float *U, float *V, float *diagonal, float *superdiagonal) {
	int i, j, k, ip1;
	float s, s2, si, scale;
	float *pu, *pui, *pv, *pvi;
	float half_norm_squared;

	// Copy A to U
	memcpy(U, A, sizeof(float) * nrows * ncols);

	diagonal[0] = 0.0;
	s = 0.0;
	scale = 0.0;
	for (i = 0, pui = U, ip1 = 1; i < ncols; pui += ncols, i++, ip1++) {
		superdiagonal[i] = scale * s;
		//
		//       Perform Householder transform on columns.
		//
		//       Calculate the normed squared of the i-th column vector starting at
		//       row i.
		//
		for (j = i, pu = pui, scale = 0.0; j < nrows; j++, pu += ncols)
			scale += fabsf(*(pu + i));

		if (scale > 0.0) {
			for (j = i, pu = pui, s2 = 0.0; j < nrows; j++, pu += ncols) {
				*(pu + i) /= scale;
				s2 += *(pu + i) * *(pu + i);
			}
			//
			//
			//       Chose sign of s which maximizes the norm
			//
			s = (*(pui + i) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + i) * s - s2;
			//
			//       Transform remaining columns by the Householder transform.
			//
			*(pui + i) -= s;

			for (j = ip1; j < ncols; j++) {
				for (k = i, si = 0.0, pu = pui; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si /= half_norm_squared;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols) {
					*(pu + j) += si * *(pu + i);
				}
			}
		}
		for (j = i, pu = pui; j < nrows; j++, pu += ncols)
			*(pu + i) *= scale;
		diagonal[i] = s * scale;
		//
		//       Perform Householder transform on rows.
		//
		//       Calculate the normed squared of the i-th row vector starting at
		//       column i.
		//
		s = 0.0;
		scale = 0.0;
		if (i >= nrows || i == (ncols - 1))
			continue;
		for (j = ip1; j < ncols; j++)
			scale += fabsf(*(pui + j));
		if (scale > 0.0) {
			for (j = ip1, s2 = 0.0; j < ncols; j++) {
				*(pui + j) /= scale;
				s2 += *(pui + j) * *(pui + j);
			}
			s = (*(pui + ip1) < 0.0) ? sqrtf(s2) : -sqrtf(s2);
			//
			//       Calculate -2/u'u
			//
			half_norm_squared = *(pui + ip1) * s - s2;
			//
			//       Transform the rows by the Householder transform.
			//
			*(pui + ip1) -= s;
			for (k = ip1; k < ncols; k++)
				superdiagonal[k] = *(pui + k) / half_norm_squared;
			if (i < (nrows - 1)) {
				for (j = ip1, pu = pui + ncols; j < nrows; j++, pu += ncols) {
					for (k = ip1, si = 0.0; k < ncols; k++)
						si += *(pui + k) * *(pu + k);
					for (k = ip1; k < ncols; k++) {
						*(pu + k) += si * superdiagonal[k];
					}
				}
			}
			for (k = ip1; k < ncols; k++)
				*(pui + k) *= scale;
		}
	}

	// Update V
	pui = U + ncols * (ncols - 2);
	pvi = V + ncols * (ncols - 1);
	*(pvi + ncols - 1) = 1.0;
	s = superdiagonal[ncols - 1];
	pvi -= ncols;
	for (i = ncols - 2, ip1 = ncols - 1; i >= 0; i--, pui -= ncols, pvi -= ncols, ip1--) {
		if (s != 0.0) {
			pv = pvi + ncols;
			for (j = ip1; j < ncols; j++, pv += ncols)
				*(pv + i) = (*(pui + j) / *(pui + ip1)) / s;
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					si += *(pui + k) * *(pv + j);
				for (k = ip1, pv = pvi + ncols; k < ncols; k++, pv += ncols)
					*(pv + j) += si * *(pv + i);
			}
		}
		pv = pvi + ncols;
		for (j = ip1; j < ncols; j++, pv += ncols) {
			*(pvi + j) = 0.0;
			*(pv + i) = 0.0;
		}
		*(pvi + i) = 1.0;
		s = superdiagonal[i];
	}

	// Update U

	pui = U + ncols * (ncols - 1);
	for (i = ncols - 1, ip1 = ncols; i >= 0; ip1 = i, i--, pui -= ncols) {
		s = diagonal[i];
		for (j = ip1; j < ncols; j++)
			*(pui + j) = 0.0;
		if (s != 0.0) {
			for (j = ip1; j < ncols; j++) {
				si = 0.0;
				pu = pui + ncols;
				for (k = ip1; k < nrows; k++, pu += ncols)
					si += *(pu + i) * *(pu + j);
				si = (si / *(pui + i)) / s;
				for (k = i, pu = pui; k < nrows; k++, pu += ncols)
					*(pu + j) += si * *(pu + i);
			}
			for (j = i, pu = pui; j < nrows; j++, pu += ncols) {
				*(pu + i) /= s;
			}
		} else
			for (j = i, pu = pui; j < nrows; j++, pu += ncols)
				*(pu + i) = 0.0;
		*(pui + i) += 1.0;
	}
}
                                                                       //
static int Givens_Reduction_to_Diagonal_Form(uint16_t nrows, uint16_t ncols, float *U, float *V, float *diagonal, float *superdiagonal) {

	float epsilon;
	float c, s;
	float f, g, h;
	float x, y, z;
	float *pu, *pv;
	int i, j, k, m;
	int rotation_test;
	int iteration_count;

	for (i = 0, x = 0.0; i < ncols; i++) {
		y = fabsf(diagonal[i]) + fabsf(superdiagonal[i]);
		if (x < y)
			x = y;
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
				if (fabsf(diagonal[m - 1]) <= epsilon)
					break;
			}
			if (rotation_test) {
				c = 0.0;
				s = 1.0;
				for (i = m; i <= k; i++) {
					f = s * superdiagonal[i];
					superdiagonal[i] *= c;
					if (fabsf(f) <= epsilon)
						break;
					g = diagonal[i];
					h = sqrtf(f * f + g * g);
					diagonal[i] = h;
					c = g / h;
					s = -f / h;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = *(pu + m - 1);
						z = *(pu + i);
						*(pu + m - 1) = y * c + z * s;
						*(pu + i) = -y * s + z * c;
					}
				}
			}
			z = diagonal[k];
			if (m == k) {
				if (z < 0.0) {
					diagonal[k] = -z;
					for (j = 0, pv = V; j < ncols; j++, pv += ncols)
						*(pv + k) = -*(pv + k);
				}
				break;
			} else {
				if (iteration_count >= MAX_ITERATION_COUNT_SVD)
					return -1;
				iteration_count++;
				x = diagonal[m];
				y = diagonal[k - 1];
				g = superdiagonal[k - 1];
				h = superdiagonal[k];
				f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
				g = sqrtf(f * f + 1.0);
				if (f < 0.0)
					g = -g;
				f = ((x - z) * (x + z) + h * (y / (f + g) - h)) / x;
				// Next QR Transformtion
				c = 1.0;
				s = 1.0;
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
						x = *(pv + i - 1);
						z = *(pv + i);
						*(pv + i - 1) = x * c + z * s;
						*(pv + i) = -x * s + z * c;
					}
					z = sqrtf(f * f + h * h);
					diagonal[i - 1] = z;
					if (z != 0.0) {
						c = f / z;
						s = h / z;
					}
					f = c * g + s * y;
					x = -s * g + c * y;
					for (j = 0, pu = U; j < nrows; j++, pu += ncols) {
						y = *(pu + i - 1);
						z = *(pu + i);
						*(pu + i - 1) = c * y + s * z;
						*(pu + i) = -s * y + c * z;
					}
				}
				superdiagonal[m] = 0.0;
				superdiagonal[k] = f;
				diagonal[k] = x;
			}
		}
	}
	return 0;
}

static void Sort_by_Decreasing_Singular_Values(uint16_t nrows, uint16_t ncols, float *singular_values, float *U, float *V) {
	int i, j, max_index;
	float temp;
	float *p1, *p2;

	for (i = 0; i < ncols - 1; i++) {
		max_index = i;
		for (j = i + 1; j < ncols; j++)
			if (singular_values[j] > singular_values[max_index])
				max_index = j;
		if (max_index == i)
			continue;
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
