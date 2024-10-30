/*
 * eig.c
 *
 *  Created on: 25 mars 2020
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /* Regular routines */
bool eig_regular(float A[], float dr[], float di[], float wr[], float wi[], size_t row);
bool eig_sym(float A[], float d[], float wr[], size_t row);

/* Include LAPACK routines */
#ifdef CLAPACK_USED
#include "CLapack/clapack.h"
#endif

/*
 * A[m*n]
 * dr[m] - Real eigenvalues 
 * di[m] - Imaginary eigenvalues
 * wr[m*n] - Real eigenvectors
 * wi[m*n] - Imaginary eigenvectors
 * m == n
 * Return true = success
 * Return false = fail
 */
bool eig(float A[], float dr[], float di[], float wr[], float wi[], size_t row) {
	/* Check if matrix is symmetric */
	bool symmetric = issymmetric(A, row, row);

	/* Status flag */
	bool status = false;

#ifdef CLAPACK_USED
	if (symmetric) {
		/* Compute the SVD is the same for EIG for a symmetric matrix */
		status = svd(A, row, row, wr, dr, wi);
		memset(wi, 0, row * row * sizeof(float));
		memset(di, 0, row * sizeof(float));
	}else{
		/* Settings */
		integer n = row, lda = row, ldvl = row, ldvr = row, info, lwork;
		real wkopt;

		/* Eigenvectors */
		real* vl = (real*)malloc(n * lda * sizeof(real));
		real* vr = (real*)malloc(n * lda * sizeof(real));

		/* Allocate memory */
		lwork = -1;
		sgeev_("V", "N", &n, A, &lda, dr, di, vl, &ldvl, vr, &ldvr, &wkopt, &lwork, &info);
		lwork = (integer)wkopt;
		real* work = (real*)malloc(lwork * sizeof(real));

		/* Compute  */
		sgeev_("V", "N", &n, A, &lda, dr, di, vl, &ldvl, vr, &ldvr, work, &lwork, &info);

		/* Check status */
		status = info == 0;

		/* Fill the eigenvectors in row major */
		size_t i, j, s = 0, t = 0;
		memset(wi, 0, row * row * sizeof(float));
		for (i = 0; i < n; i++) {
			if (fabsf(di[i]) < MIN_VALUE) {
				s = t;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[s++];
				}
				t = s;
			}
			else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[t++];
				}
				for (j = 0; j < row; j++) {
					wi[j * row + i] = vl[t++];
				}
			}
		}

		/* Free */
		free(work);
		free(vl);
		free(vr);
	}
#elif defined(MKL_LAPACK_USED)
	if (symmetric) {
		/* Compute the SVD is the same for EIG for a symmetric matrix */
		status = svd(A, row, row, wr, dr, wi);
		memset(wi, 0, row * row * sizeof(float));
		memset(di, 0, row * sizeof(float));
	}
	else {
		/* Create copy */
		size_t i = row * row * sizeof(float);
		float* A_copy = (float*)malloc(i);
		memcpy(A_copy, A, i);

		/* Eigenvalues and eigenvectors */
		float* vl = (float*)malloc(i);
		float* vr = (float*)malloc(i);

		/* Compute*/
		status = LAPACKE_sgeev(LAPACK_COL_MAJOR, 'V', 'N', row, A_copy, row, dr, di, vl, row, vr, row) == 0;
		
		/* Fill the eigenvectors */
		size_t j, s = 0, t = 0;
		memset(wi, 0, row * row * sizeof(float));
		for (i = 0; i < row; i++) {
			if (fabsf(di[i]) < MIN_VALUE) {
				s = t;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[s++];
				}
				t = s;
			}
			else {
				t = s;
				for (j = 0; j < row; j++) {
					wr[j * row + i] = vl[t++];
				}
				for (j = 0; j < row; j++) {
					wi[j * row + i] = vl[t++];
				}
			}
		}

		/* Free */
		free(A_copy);
		free(vl);
		free(vr);
	}
#else
	if (symmetric) {
		status = eig_sym(A, dr, wr, row);
		memset(wi, 0, row * row * sizeof(float));
		memset(di, 0, row * sizeof(float));
	}
	else {
		status = eig_regular(A, dr, di, wr, wi, row);
	}
#endif

	/* Return status */
	return status;
}

static bool tqli(float d[], float e[], size_t row, float z[]);
static void tridiag(float A[], size_t row, float d[], float e[]);
static float pythag_float(float a, float b);
#define square(a) ((a)*(a))
#define abs_sign(a,b) ((b) >= 0.0f ? fabsf(a) : -fabsf(a)) /* Special case for tqli function */

bool eig_sym(float A[], float d[], float wr[], size_t row) {
	/* Copy A - wr will be eigenvectors */
	memcpy(wr, A, row * row * sizeof(float));

	float* e = (float*)malloc(row * sizeof(float));
	memset(e, 0, row * sizeof(float));
	memset(d, 0, row * sizeof(float));
	tridiag(wr, row, d, e);
	bool status = tqli(d, e, row, wr);

	/* Free */
	free(e);

	/* Return status */
	return status;
}

/* Create a tridiagonal matrix */
static void tridiag(float A[], size_t row, float d[], float e[]) {
	size_t l, k, j, i;
	float scale, hh, h, g, f;

	/* Save address */
	float* Aj, * Ak, * Ai;
	Ai = A;
	Ai += row * (row - 1);
	for (i = row - 1; i > 0; i--) {
		l = i - 1;
		h = scale = 0.0f;
		if (l > 0) {
			for (k = 0; k < l + 1; k++) {
				scale += fabsf(Ai[k]); /* scale += fabsf(*(A + row*i + k)); */
			}
			if (scale == 0.0f) {
				e[i] = Ai[l]; /* *(e + i) = *(A + row*i + l); */
			}
			else {
				for (k = 0; k < l + 1; k++) {
					Ai[k] /= scale;
					/* *(A + row*i + k) /= scale; */
					h += Ai[k] * Ai[k];
					/* h += *(A + row*i + k) * *(A + row*i + k); */
				}
				f = Ai[l]; /* *(A + row*i + l) */
				g = (f >= 0.0f ? -sqrtf(h) : sqrtf(h));
				e[i] = scale * g;
				h -= f * g;
				Ai[l] = f - g; /* *(A + row*i + l) = f - g; */
				f = 0.0f;
				Aj = A;
				for (j = 0; j < l + 1; j++) {
					/* Next statement can be omitted if eigenvectors not wanted */
					Aj[i] = Ai[j] / h;
					/* *(A + row*j + i) = *(A + row*i + j) / h; */
					g = 0.0f;
					for (k = 0; k < j + 1; k++) {
						g += Aj[k] * Ai[k]; /* g += *(A + row*j + k) * *(A + row*i + k); */
					}
					Ak = A;
					Ak += row * j; /* Important that is is row*j and not row*(j+1) **/
					for (k = j + 1; k < l + 1; k++) {
						Ak += row; /* Here is the +1 */
						g += Ak[j] * Ai[k];
						/* g += *(A + row*k + j) * *(A + row*i + k); */
					}
					e[j] = g / h;
					f += e[j] * Ai[j];
					Aj += row;
				}
				hh = f / (h + h);
				Aj = A;
				for (j = 0; j < l + 1; j++) { /* l + 1 */
					f = Ai[j]; /* *(A + row*i + j) */
					e[j] = g = e[j] - hh * f;
					for (k = 0; k < j + 1; k++) {
						Aj[k] -= (f * e[k] + g * Ai[k]); /* *(A + row*j + k) -= (f * e[k] + g * *(A + row*i + k)); */
					}
					Aj += row;
				}
			}
		}
		else {
			e[i] = Ai[l]; /* *(e + i) = *(A + row*i + l);//a[i][l]; */
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
				for (k = 0; k < l; k++) {
					g += Ai[k] * Ak[j];
					Ak += row;
					/* g += *(A + row*i + k) * *(A + row*k + j) */
				}
				Ak = A;
				for (k = 0; k < l; k++) {
					Ak[j] -= g * Ak[i];
					Ak += row;
					/* *(A + row*k + j) -= g * *(A + row*k + i) */
				}
			}
		}
		d[i] = Ai[i]; /* *(A + row*i + i) */
		Ai[i] = 1.0f; /* *(A + row*i + i) = 1.0 */
		Aj = A;
		for (j = 0; j < l; j++) {
			Aj[i] = Ai[j] = 0.0f;
			Aj += row;
			/* *(A + row*j + i) = *(A + row*i + j) = 0.0; */
		}

		Ai += row;
	}
}

static float pythag_float(float a, float b) {
	float absa = fabsf(a);
	float absb = fabsf(b);
	if (absa > absb) {
		return absa * sqrtf(1.0f + square(absb / absa));
	}
	else {
		return (absb < MIN_VALUE ? 0.0f : absb * sqrtf(1.0f + square(absa / absb)));
	}
}

static bool tqli(float d[], float e[], size_t row, float z[]) {
	int32_t m, l, iter, i, k;
	float s, r, p, g, f, dd, c, b;
	bool ok = true;
	/* Save address */
	float* zk;

	for (i = 1; i < row; i++) {
		e[i - 1] = e[i];
	}
	e[row - 1] = 0.0f;
	for (l = 0; l < row; l++) {
		iter = 0;
		do {
			for (m = l; m < row - 1; m++) {
				dd = fabsf(d[m]) + fabsf(d[m + 1]);
				if (fabsf(e[m]) < MIN_VALUE) {
					break;
				}
			}
			if (m != l) {
				if (iter++ == MAX_ITERATIONS) {
					/* Too many iterations */
					ok = false;
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
					if (fabsf(r) < MIN_VALUE) {
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
						f = zk[i + 1];
						zk[i + 1] = s * zk[i] + c * f;
						zk[i] = c * zk[i] - s * f;
						zk += row;
						/* f = *(z + row*k + i + 1) */
						/* *(z + row*k + i + 1) = s * *(z + row*k + i) + c * f */
						/* *(z + row*k + i) = c * *(z + row*k + i) - s * f */
					}
				}
				if (r == 0.0f && i >= l) {
					continue;
				}
				d[l] -= p;
				e[l] = g;
				e[m] = 0.0f;
			}
		} while (m != l);
	}
	return ok;
}

static bool qr_shift_algorithm(float A[], float dr[], float di[], size_t row);
static void prepare(float A[], size_t row);
/*#define abs_sign(a,b) ((b) >= 0.0 ? fabsf(a) : -fabsf(a))  Special case for qr_hess function */

/*
 * Find eigenvalues from a general matrix
 * A [m*n]
 * dr [m]
 * di [m]
 * wr [m*n]
 * wi [m*n]
 * n == m
 */
bool eig_regular(float A[], float dr[], float di[], float wr[], float wi[], size_t row) {
	/* Copy A */
	float* Acopy = (float*)malloc(row * row * sizeof(float));
	memcpy(Acopy, A, row * row * sizeof(float));

	/* Balance and prepare Acopy */
	balance(Acopy, row);
	prepare(Acopy, row);

	/* Reset before finding eigenvetors */
	memset(dr, 0, row * sizeof(float));
	memset(di, 0, row * sizeof(float));

	/* Compute the eigenvalues dr, di inside A */
	bool status = qr_shift_algorithm(Acopy, dr, di, row);

	/* TODO: Compute the eigenvectors */

	/* Free */
	free(Acopy);

	/* Return status */
	return status;
}


/* Prepare the matrix A for the QR algorithm */
static void prepare(float A[], size_t row) {
	size_t i, j, m;
	float y, x, t;

	for (m = 1; m < row; m++) {
		x = 0.0f;
		i = m;
		for (j = m; j < row; j++) {
			if (fabsf(A[row * j + m - 1]) > fabsf(x)) {
				x = *(A + row * j + m - 1);
				i = j;
			}
		}
		if (i != m) {
			for (j = m - 1; j < row; j++) {
				/* Do swap */
				t = *(A + row * i + j);
				A[row * i + j] = A[row * m + j];
				A[row * m + j] = t;
			}
			for (j = 0; j < row; j++) {
				/* Do swap */
				t = *(A + row * j + i);
				A[row * j + i] = A[row * j + m];
				A[row * j + m] = t;
			}
		}
		if (x != 0.0) {
			for (i = m + 1; i < row; i++) {
				if (fabsf(y = A[row * i + m - 1]) > MIN_VALUE) {
					y /= x;
					A[row * i + m - 1] = y;
					for (j = m; j < row; j++) {
						A[row * i + j] -= y * A[row * m + j];
					}
					for (j = 0; j < row; j++) {
						A[row * j + m] += y * A[row * j + i];
					}
				}
			}
		}
	}
}

/* Shifted QR */
static bool qr_shift_algorithm(float A[], float wr[], float wi[], size_t row) {
	int32_t nn, m, l, k, j, its, i, mmin;
	float z, y, x, w, v, u, t, s, r, q, p, anorm;
	bool ok = true;
	p = q = r = 0.0f;
	anorm = 0.0f;
	for (i = 0; i < row; i++)
		for (j = i - 1 > 0 ? i - 1 : 0; j < row; j++) {
			anorm += fabsf(*(A + row * i + j));
		}
	nn = row - 1;
	t = 0.0f;
	while (nn >= 0) {
		its = 0;
		do {
			for (l = nn; l > 0; l--) {
				s = fabsf(A[row * (l - 1) + l - 1]) + fabsf(A[row * l + l]);
				if (s == 0.0f) {
					s = anorm;
				}
				if (fabsf(A[row * l + l - 1]) < MIN_VALUE) {
					A[row * l + l - 1] = 0.0f;
					break;
				}
			}
			x = A[row * nn + nn];
			if (l == nn) {
				wr[nn] = x + t;
				wi[nn--] = 0.0f;
			}
			else {
				y = A[row * (nn - 1) + nn - 1];
				w = A[row * nn + nn - 1] * A[row * (nn - 1) + nn];
				if (l == nn - 1) {
					p = 0.5f * (y - x);
					q = p * p + w;
					z = sqrtf(fabsf(q));
					x += t;
					if (q >= 0.0f) {
						z = p + abs_sign(z, p);
						wr[nn - 1] = wr[nn] = x + z;
						if (fabsf(z) > MIN_VALUE) {
							*(wr + nn) = x - w / z;
						}
						wi[nn - 1] = wi[nn] = 0.0f;
					}
					else {
						wr[nn - 1] = wr[nn] = x + p;
						wi[nn - 1] = -(wi[nn] = z);
					}
					nn -= 2;
				}
				else {
					if (its >= MAX_ITERATIONS) {
						/* Too many iterations */
						ok = false;
						break;
					}
					if (its == 10 || its == 20) {
						t += x;
						for (i = 0; i < nn + 1; i++) {
							A[row * i + i] -= x;
						}
						s = fabsf(A[row * nn + nn - 1]) + fabsf(A[row * (nn - 1) + nn - 2]);
						y = x = 0.75f * s;
						w = -0.4375f * s * s;
					}
					++its;
					for (m = nn - 2; m >= l; m--) {
						z = A[row * m + m];
						r = x - z;
						s = y - z;
						p = (r * s - w) / A[row * (m + 1) + m] + A[row * m + m + 1];
						q = A[row * (m + 1) + m + 1] - z - r - s;
						r = A[row * (m + 2) + m + 1];
						s = fabsf(p) + fabsf(q) + fabsf(r);
						p /= s;
						q /= s;
						r /= s;
						if (m == l) {
							break;
						}
						u = fabsf(A[row * m + m - 1]) * (fabsf(q) + fabsf(r));
						v = fabsf(p) * (fabsf(A[row * (m - 1) + m - 1]) + fabsf(z) + fabsf(A[row * (m + 1) + m + 1]));
						if (fabsf(u) < MIN_VALUE) {
							break;
						}
					}
					for (i = m; i < nn - 1; i++) {
						A[row * (i + 2) + i] = 0.0f;
						if (i != m) {
							A[row * (i + 2) + i - 1] = 0.0f;
						}
					}
					for (k = m; k < nn; k++) {
						if (k != m) {
							p = A[row * k + k - 1];
							q = A[row * (k + 1) + k - 1];
							r = 0.0f;
							if (k != nn - 1) {
								r = A[row * (k + 2) + k - 1];
							}
							if (fabsf(x = fabsf(p) + fabsf(q) + fabsf(r)) > MIN_VALUE) {
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if (fabsf(s = abs_sign(sqrtf(p * p + q * q + r * r), p)) > MIN_VALUE) {
							if (k == m) {
								if (l != m) {
									A[row * k + k - 1] = -A[row * k + k - 1];
								}
							}
							else {
								A[row * k + k - 1] = -s * x;
							}
							p += s;
							x = p / s;
							y = q / s;
							z = r / s;
							q /= p;
							r /= p;
							for (j = k; j < nn + 1; j++) {
								p = A[row * k + j] + q * A[row * (k + 1) + j];
								if (k != nn - 1) {
									p += r * A[row * (k + 2) + j];
									A[row * (k + 2) + j] -= p * z;
								}
								A[row * (k + 1) + j] -= p * y;
								A[row * k + j] -= p * x;
							}
							mmin = nn < k + 3 ? nn : k + 3;
							for (i = l; i < mmin + 1; i++) {
								p = x * A[row * i + k] + y * A[row * i + k + 1];
								if (k != (nn - 1)) {
									p += z * A[row * i + k + 2];
									A[row * i + k + 2] -= p * r;
								}
								A[row * i + k + 1] -= p * q;
								A[row * i + k] -= p;
							}
						}
					}
				}
			}
		} while (l < nn - 1);
	}
	return ok;
}
