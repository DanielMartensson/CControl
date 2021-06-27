/*
 * eig.c
 *
 *  Created on: 25 mars 2020
 *      Author: hp
 */

#include "../../Headers/Functions.h"

static void qr_shift_algorithm(float* A, float* wr, float* wi, uint16_t row);
static void prepare(float* A, uint16_t row);
#define abs_sign(a,b) ((b) >= 0.0 ? fabsf(a) : -fabsf(a)) // Special case for qr_hess function

/*
 * Find eigenvalues from a general matrix
 * A [m*n] // Will be changed
 * wr [m] // Real eigenvalues
 * wi [m] // Imaginary eigenvalues
 * n == m
 */
void eig(float A[], float wr[], float wi[], uint16_t row){
	// Find the eigenvalues
	balance(A, row);
	prepare(A, row);
	// Reset before
	memset(wr, 0, row*sizeof(float));
	memset(wi, 0, row*sizeof(float));
	qr_shift_algorithm(A, wr, wi, row);

}

// Prepare the matrix A for the QR algorithm
static void prepare(float* A, uint16_t row){
	int i, j, m;
	float y, x, t;

	for (m = 1; m < row - 1; m++) {
		x = 0.0;
		i = m;
		for (j = m; j < row; j++) {
			if (fabsf(*(A + row*j + m-1)) > fabsf(x)) {
				x = *(A + row*j + m-1);
				i = j;
			}
		}
		if (i != m) {
			for (j = m - 1; j < row; j++){
				// Do swap
				t = *(A + row*i + j);
				*(A + row*i + j) = *(A + row*m + j);
				*(A + row*m + j) = t;
			}
			for (j = 0; j < row; j++){
				// Do swap
				t = *(A + row*j + i);
				*(A + row*j + i) = *(A + row*j + m);
				*(A + row*j + m) = t;
			}
		}
		if (x != 0.0) {
			for (i = m + 1; i < row; i++) {
				if ((y = *(A + row*i + m-1)) != 0.0) {
					y /= x;
					*(A + row*i + m-1) = y;
					for (j = m; j < row; j++)
						*(A + row*i + j) -= y * *(A + row*m + j);
					for (j = 0; j < row; j++)
						*(A + row*j + m) += y * *(A + row*j + i);
				}
			}
		}
	}
}

// Shifted QR
static void qr_shift_algorithm(float* A, float* wr, float* wi, uint16_t row){
	int nn, m, l, k, j, its, i, mmin;
	float z, y, x, w, v, u, t, s, r, q, p, anorm;

	p = q = r = 0.0;
	anorm = 0.0;
	for (i = 0; i < row; i++)
		for (j = i - 1 > 0 ? i - 1 : 0; j < row; j++)
			anorm += fabsf(*(A + row*i + j));
	nn = row - 1;
	t = 0.0;
	while (nn >= 0) {
		its = 0;
		do {
			for (l = nn; l > 0; l--) {
				s = fabsf(*(A + row*(l-1) + l-1)) + fabsf(*(A + row*l + l));
				if (s == 0.0)
					s = anorm;
				if (fabsf(*(A + row*l + l-1)) + s == s) {
					*(A + row*l + l-1) = 0.0;
					break;
				}
			}
			x = *(A + row*nn + nn);
			if (l == nn) {
				*(wr + nn) = x + t;
				*(wi + nn--) = 0.0;
			} else {
				y = *(A + row*(nn-1) + nn-1);
				w = *(A + row*nn + nn-1) * *(A + row*(nn-1) + nn);
				if (l == nn - 1) {
					p = 0.5 * (y - x);
					q = p * p + w;
					z = sqrtf(fabsf(q));
					x += t;
					if (q >= 0.0) {
						z = p + abs_sign(z, p);
						*(wr + nn -1) = *(wr + nn) = x + z;
						if (z != 0.0)
							*(wr + nn) = x - w / z;
						*(wi + nn -1) = *(wi + nn) = 0.0;
					} else {
						*(wr + nn -1) = *(wr + nn) = x + p;
						*(wi + nn -1) = -(*(wi + nn) = z);
					}
					nn -= 2;
				} else {
					if (its == 30) {
						//fprintf(stderr, "[hqr] too many iterations.\n");
						break;
					}
					if (its == 10 || its == 20) {
						t += x;
						for (i = 0; i < nn + 1; i++)
							*(A + row*i + i) -= x;
						s = fabsf(*(A + row*nn + nn-1)) + fabsf(*(A + row*(nn-1) + nn-2));
						y = x = 0.75 * s;
						w = -0.4375 * s * s;
					}
					++its;
					for (m = nn - 2; m >= l; m--) {
						z = *(A + row*m + m);
						r = x - z;
						s = y - z;
						p = (r * s - w) / *(A + row*(m+1) + m) + *(A + row*m + m+1);
						q = *(A + row*(m+1) + m+1) - z - r - s;
						r = *(A + row*(m+2) + m+1);
						s = fabsf(p) + fabsf(q) + fabsf(r);
						p /= s;
						q /= s;
						r /= s;
						if (m == l)
							break;
						u = fabsf(*(A + row*m + m-1)) * (fabsf(q) + fabsf(r));
						v = fabsf(p) * (fabsf(*(A + row*(m-1) + m-1)) + fabsf(z) + fabsf(*(A + row*(m+1) + m+1)));
						if (u + v == v)
							break;
					}
					for (i = m; i < nn - 1; i++) {
						*(A + row*(i+2) + i) = 0.0;
						if (i != m)
							*(A + row*(i+2) + i-1) = 0.0;
					}
					for (k = m; k < nn; k++) {
						if (k != m) {
							p = *(A + row*k + k-1);
							q = *(A + row*(k+1) + k-1);
							r = 0.0;
							if (k + 1 != nn)
								r = *(A + row*(k+2) + k-1);
							if ((x = fabsf(p) + fabsf(q) + fabsf(r)) != 0.0) {
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if ((s = abs_sign(sqrtf(p * p + q * q + r * r), p)) != 0.0) {
							if (k == m) {
								if (l != m)
									*(A + row*k + k-1) = - *(A + row*k + k-1);
							} else
								*(A + row*k + k-1) = -s * x;
							p += s;
							x = p / s;
							y = q / s;
							z = r / s;
							q /= p;
							r /= p;
							for (j = k; j < nn + 1; j++) {
								p = *(A + row*k + j) + q * *(A + row*(k+1) + j);
								if (k + 1 != nn) {
									p += r * *(A + row*(k+2) + j);
									*(A + row*(k+2) + j) -= p * z;
								}
								*(A + row*(k+1) + j) -= p * y;
								*(A + row*k + j) -= p * x;
							}
							mmin = nn < k + 3 ? nn : k + 3;
							for (i = l; i < mmin + 1; i++) {
								p = x * *(A + row*i + k) + y * *(A + row*i + k+1);
								if (k != (nn)) {
									p += z * *(A + row*i + k+2);
									*(A + row*i + k+2) -= p * r;
								}
								*(A + row*i + k+1) -= p * q;
								*(A + row*i + k) -= p;
							}
						}
					}
				}
			}
		} while (l + 1 < nn);
	}
}
