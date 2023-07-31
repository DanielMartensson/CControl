#ifndef LAPACK
#define LAPACK

#include "f2c.h"

/* Eigenvalues of an symmetric matrix */
int ssyevd_(char* jobz, char* uplo, integer* n, real* a,
	integer* lda, real* w, real* work, integer* lwork, integer* iwork,
	integer* liwork, integer* info);

/* Eigenvalues of a square genereal matrix */
int sgeev_(char* jobvl, char* jobvr, integer* n, real* a,
	integer* lda, real* wr, real* wi, real* vl, integer* ldvl, real* vr,
	integer* ldvr, real* work, integer* lwork, integer* info);

/* SVD of a general matrix */
int sgesvd_(char* jobu, char* jobvt, integer* m, integer* n,
	real* a, integer* lda, real* s, real* u, integer* ldu, real* vt,
	integer* ldvt, real* work, integer* lwork, integer* info);

/* Cholesky factorization */
int spotrf_(char* uplo, integer* n, real* a, integer* lda, integer* info);

#endif // !LAPACK
