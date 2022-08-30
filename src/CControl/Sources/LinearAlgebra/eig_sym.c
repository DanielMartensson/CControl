/*
 * eig_sym.c
 *
 *  Created on: 29 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/Functions.h"

// Private functions
static void tqli(float d[], float e[], uint16_t row, float z[]);
static void tridiag(float A[], uint16_t row, float d[], float e[]);
static float pythag_float(float a, float b);
#define square(a) ((a)*(a))
#define abs_sign(a,b) ((b) >= 0.0f ? fabsf(a) : -fabsf(a)) // Special case for tqli function


/*
 * Compute eigenvalues and eigenvectors from a symmetrical square matrix A
 * Notice that a square symmetrical matrix can never have complex eigenvalues and eigenvalues!
 * A [m*n]
 * n == m
 * A^T = A
 * d [m] // Eigenvalues
 * A will become eigenvectors!
 */
void eig_sym(float A[], uint16_t row, float d[]){
	float e[row];
	memset(e, 0, row*sizeof(float));
	memset(d, 0, row*sizeof(float));
	tridiag(A, row, d, e);
	tqli(d, e, row, A);
}


// Create a tridiagonal matrix
static void tridiag(float A[], uint16_t row, float d[], float e[]){
	uint16_t l, k, j, i;
	float scale, hh, h, g, f;

	// Save address
	float *Aj, *Ak, *Ai;
	Ai = A;
	Ai += row*(row - 1);
	for (i = row - 1; i > 0; i--) {
		l = i - 1;
		h = scale = 0.0f;
		if (l > 0) {
			for (k = 0; k < l + 1; k++)
				scale += fabsf(Ai[k]); //scale += fabsf(*(A + row*i + k));
			if (scale == 0.0f)
				e[i] = Ai[l]; //*(e + i) = *(A + row*i + l);
			else {
				for (k = 0; k < l + 1; k++) {
					Ai[k] /= scale;
					//*(A + row*i + k) /= scale;
					h += Ai[k] * Ai[k];
					// h += *(A + row*i + k) * *(A + row*i + k);
				}
				f = Ai[l]; // *(A + row*i + l)
				g = (f >= 0.0f ? -sqrtf(h) : sqrtf(h));
				e[i] = scale * g;
				h -= f * g;
				Ai[l] = f - g; //*(A + row*i + l) = f - g;
				f = 0.0f;
				Aj = A;
				for (j = 0; j < l + 1; j++) {
					/* Next statement can be omitted if eigenvectors not wanted */
					Aj[i] = Ai[j] / h;
					//*(A + row*j + i) = *(A + row*i + j) / h;
					g = 0.0f;
					for (k = 0; k < j + 1; k++)
						g += Aj[k] * Ai[k]; //g += *(A + row*j + k) * *(A + row*i + k);
					Ak = A;
					Ak += row*j; // Important that is is row*j and not row*(j+1)
					for (k = j + 1; k < l + 1; k++){
						Ak += row; // Here is the +1
						g += Ak[j] * Ai[k];
						//g += *(A + row*k + j) * *(A + row*i + k);
					}
					e[j] = g / h;
					f += e[j] * Ai[j];
					Aj += row;
				}
				hh = f / (h + h);
				Aj = A;
				for (j = 0; j < l + 1; j++) { // l + 1
					f = Ai[j]; // *(A + row*i + j)
					e[j] = g = e[j] - hh * f;
					for (k = 0; k < j + 1; k++)
						Aj[k] -= (f * e[k] + g * Ai[k]); // *(A + row*j + k) -= (f * e[k] + g * *(A + row*i + k));
					Aj += row;
				}
			}
		} else{
			e[i] = Ai[l]; //*(e + i) = *(A + row*i + l);//a[i][l];
		}
		d[i] = h;

		Ai -= row;
	}
	/* Next statement can be omitted if eigenvectors not wanted */
	d[0] = 0.0f;
	e[0] = 0.0f;
	/* Contents of this loop can be omitted if eigenvectors not wanted except for statement d[i]=a[i][i]; */
	Ai = A;
	for (i = 0; i < row; i++) {
		l = i;
		if (d[i] != 0.0f) {
			for (j = 0; j < l; j++) {
				g = 0.0;
				Ak = A;
				for (k = 0; k < l; k++){
					g += Ai[k] * Ak[j];
					Ak += row;
					//g += *(A + row*i + k) * *(A + row*k + j)
				}
				Ak = A;
				for (k = 0; k < l; k++){
					Ak[j] -= g * Ak[i];
					Ak += row;
					//*(A + row*k + j) -= g * *(A + row*k + i)
				}
			}
		}
		d[i] = Ai[i]; // *(A + row*i + i)
		Ai[i] = 1.0f; //*(A + row*i + i) = 1.0
		Aj = A;
		for (j = 0; j < l; j++){
			Aj[i] = Ai[j] = 0.0f;
			Aj += row;
			//*(A + row*j + i) = *(A + row*i + j) = 0.0;
		}

		Ai += row;
	}
}

static float pythag_float(float a, float b){
	float absa = fabsf(a);
	float absb = fabsf(b);
	if (absa > absb)
		return absa * sqrtf(1.0f + square(absb / absa));
	else
		return (absb == 0.0f ? 0.0f : absb * sqrtf(1.0f + square(absa / absb)));
}

static void tqli(float d[], float e[], uint16_t row, float z[]){
	int32_t m, l, iter, i, k;
	float s, r, p, g, f, dd, c, b;

	// Save address
	float *zk;

	for (i = 1; i < row; i++)
		e[i - 1] = e[i];
	e[row - 1] = 0.0f;
	for (l = 0; l < row; l++) {
		iter = 0;
		do {
			for (m = l; m < row - 1; m++) {
				dd = fabsf(d[m]) + fabsf(d[m + 1]);
				if (fabsf(e[m]) + dd == dd)
					break;
			}
			if (m != l) {
				if (iter++ == 30) {
					fprintf(stderr, "[tqli] Too many iterations in tqli.\n");
					break;
				}
				g = (d[l + 1] - d[l]) / (2.0f * e[l]);
				r = pythag_float(g, 1.0f);
				g = d[m] - d[l] + e[l] / (g + abs_sign(r, g));
				s = c = 1.0f;
				p = 0.0f;
				for (i = m - 1; i >= l; i--) {
					f = s * e[i];
					b = c * e[i];
					e[i + 1] = (r = pythag_float(f, g));
					if (r == 0.0f) {
						d[i + 1] -= p;
						e[m] = 0.0f;
						break;
					}
					s = f / r;
					c = g / r;
					g = d[i + 1] - p;
					r = (d[i] - g) * s + 2.0f * c * b;
					d[i + 1] = g + (p = s * r);
					g = c * r - b;
					/* Next loop can be omitted if eigenvectors not wanted */
					zk = z;
					for (k = 0; k < row; k++) {
						f = zk[i+1];
						zk[i+1] = s * zk[i] + c * f;
						zk[i] = c * zk[i] - s * f;
						zk += row;
						//f = *(z + row*k + i + 1)
						//*(z + row*k + i + 1) = s * *(z + row*k + i) + c * f
						//*(z + row*k + i) = c * *(z + row*k + i) - s * f
					}
				}
				if (r == 0.0f && i >= l)
					continue;
				d[l] -= p;
				e[l] = g;
				e[m] = 0.0f;
			}
		} while (m != l);
	}
}
