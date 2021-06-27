/*
 * eig_sym.c
 *
 *  Created on: 29 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

// Private functions
static void tqli(float *d, float *e, uint16_t row, float *z);
static void tridiag(float* a, uint16_t row, float *d, float *e);
static float pythag_float(float a, float b);
#define square(a) ((a)*(a))
#define abs_sign(a,b) ((b) >= 0.0 ? fabsf(a) : -fabsf(a)) // Special case for tqli function


/*
 * Compute eigenvalues and eigenvectors from a symmetrical square matrix A
 * Notice that a square symmetrical matrix can never have complex eigenvalues and eigenvalues!
 * A [m*n]
 * n == m
 * A^T = A
 * d [m] // Eigenvalues
 * A will become eigenvectors!
 */
void eig_sym(float* A, uint16_t row, float d[]){
	float e[row];
	memset(e, 0, row*sizeof(float));
	memset(d, 0, row*sizeof(float));
	tridiag(A, row, d, e);
	tqli(d, e, row, A);
}


// Create a tridiagonal matrix
static void tridiag(float* a, uint16_t row, float *d, float *e){
	int l, k, j, i;
	float scale, hh, h, g, f;

	for (i = row - 1; i > 0; i--) {
		l = i - 1;
		h = scale = 0.0;
		if (l > 0) {
			for (k = 0; k < l + 1; k++)
				scale += fabsf(*(a + row*i + k)); //fabs(a[i][k]);
			if (scale == 0.0)
				*(e + i) = *(a + row*i + l); //a[i][l];
			else {
				for (k = 0; k < l + 1; k++) {
					*(a + row*i + k) /= scale; //a[i][k] /= scale;
					h += *(a + row*i + k) * *(a + row*i + k);//a[i][k] * a[i][k];
				}
				f = *(a + row*i + l);//a[i][l];
				g = (f >= 0.0 ? -sqrtf(h) : sqrtf(h));
				*(e + i) = scale * g;
				h -= f * g;
				*(a + row*i + l) = f - g; // a[i][l]
				f = 0.0;
				for (j = 0; j < l + 1; j++) {
					/* Next statement can be omitted if eigenvectors not wanted */
					*(a + row*j + i) = *(a + row*i + j) / h;//a[j][i] = a[i][j] / h;
					g = 0.0;
					for (k = 0; k < j + 1; k++)
						g += *(a + row*j + k) * *(a + row*i + k);//a[j][k] * a[i][k];
					for (k = j + 1; k < l + 1; k++)
						g += *(a + row*k + j) * *(a + row*i + k);//a[k][j] * a[i][k];
					*(e + j) = g / h;
					f += *(e + j) * *(a + row*i + j); // a[i][j]
				}
				hh = f / (h + h);
				for (j = 0; j < l + 1; j++) { // l + 1
					f = *(a + row*i + j);//a[i][j];
					*(e + j) = g = *(e + j) - hh * f;
					for (k = 0; k < j + 1; k++)
						*(a + row*j + k) -= (f * e[k] + g * *(a + row*i + k));//a[j][k] -= (f * e[k] + g * a[i][k]);
				}
			}
		} else
			*(e + i) = *(a + row*i + l);//a[i][l];
		*(d + i) = h;
	}
	/* Next statement can be omitted if eigenvectors not wanted */
	*(d + 0) = 0.0;
	*(e + 0) = 0.0;
	/* Contents of this loop can be omitted if eigenvectors not wanted except for statement d[i]=a[i][i]; */
	for (i = 0; i < row; i++) {
		l = i;
		if (*(d + i) != 0.0) {
			for (j = 0; j < l; j++) {
				g = 0.0;
				for (k = 0; k < l; k++)
					g += *(a + row*i + k) * *(a + row*k + j);//a[i][k] * a[k][j];
				for (k = 0; k < l; k++)
					*(a + row*k + j) -= g * *(a + row*k + i);//a[k][j] -= g * a[k][i];
			}
		}
		*(d + i) = *(a + row*i + i);//a[i][i];
		*(a + row*i + i) = 1.0;//a[i][i] = 1.0;
		for (j = 0; j < l; j++)
			*(a + row*j + i) = *(a + row*i + j) = 0.0; //a[j][i] = a[i][j] = 0.0;
	}
}

static float pythag_float(float a, float b){
	float absa = fabsf(a);
	float absb = fabsf(b);
	if (absa > absb)
		return absa * sqrtf(1.0 + square(absb / absa));
	else
		return (absb == 0.0 ? 0.0 : absb * sqrtf(1.0 + square(absa / absb)));
}

static void tqli(float *d, float *e, uint16_t row, float *z){
	int m, l, iter, i, k;
	float s, r, p, g, f, dd, c, b;

	for (i = 1; i < row; i++)
		*(e + i - 1) = *(e + i);
	e[row - 1] = 0.0;
	for (l = 0; l < row; l++) {
		iter = 0;
		do {
			for (m = l; m < row - 1; m++) {
				dd = fabsf(*(d + m)) + fabsf(*(d + m + 1));
				if (fabsf(*(e + m)) + dd == dd)
					break;
			}
			if (m != l) {
				if (iter++ == 30) {
					fprintf(stderr, "[tqli] Too many iterations in tqli.\n");
					break;
				}
				g = (*(d + l + 1) - *(d + l)) / (2.0 * *(e +l));
				r = pythag_float(g, 1.0);
				g = *(d + m) - *(d + l) + *(e + l) / (g + abs_sign(r, g));
				s = c = 1.0;
				p = 0.0;
				for (i = m - 1; i >= l; i--) {
					f = s * *(e + i);
					b = c * *(e + i);
					e[i + 1] = (r = pythag_float(f, g));
					if (r == 0.0) {
						*(d + i + 1) -= p;
						*(e + m) = 0.0;
						break;
					}
					s = f / r;
					c = g / r;
					g = *(d + i + 1) - p;
					r = (*(d + i) - g) * s + 2.0 * c * b;
					*(d + i + 1) = g + (p = s * r);
					g = c * r - b;
					/* Next loop can be omitted if eigenvectors not wanted */
					for (k = 0; k < row; k++) {
						f = *(z + row*k + i + 1);//z[k][i + 1];
						*(z + row*k + i + 1) = s * *(z + row*k + i) + c * f;//z[k][i + 1] = s * z[k][i] + c * f;
						*(z + row*k + i) = c * *(z + row*k + i) - s * f;//z[k][i] = c * z[k][i] - s * f;
					}
				}
				if (r == 0.0 && i >= l)
					continue;
				*(d + l) -= p;
				*(e + l) = g;
				*(e + m) = 0.0;
			}
		} while (m != l);
	}
}
