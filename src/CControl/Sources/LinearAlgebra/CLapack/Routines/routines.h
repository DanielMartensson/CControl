#ifndef ROUTINES
#define ROUTINES

#ifdef __cplusplus
extern "C" {
#endif

#include "f2c.h"

/* Eigenvalues of a symmetric matrix */
int ssyevd_(char* jobz, char* uplo, integer* n, real* a,
	integer* lda, real* w, real* work, integer* lwork, integer* iwork,
	integer* liwork, integer* info);

/* Eigenvalues of a non-symmetric matrix */
int sgeev_(char* jobvl, char* jobvr, integer* n, real* a,
	integer* lda, real* wr, real* wi, real* vl, integer* ldvl, real* vr,
	integer* ldvr, real* work, integer* lwork, integer* info);

/* Generalized eigenvalue decomposition for a symmetric matrices */
int ssygvd_(integer* itype, char* jobz, char* uplo, integer*
	n, real* a, integer* lda, real* b, integer* ldb, real* w, real* work,
	integer* lwork, integer* iwork, integer* liwork, integer* info);

/* Generalized eigenvalue decomposition for a non-symmetric matrices */
int sggev_(char* jobvl, char* jobvr, integer* n, real* a,
	integer* lda, real* b, integer* ldb, real* alphar, real* alphai, real
	* beta, real* vl, integer* ldvl, real* vr, integer* ldvr, real* work,
	integer* lwork, integer* info);

/* SVD of a general matrix by using QR factorization - Used when you want to compute only U or V */
int sgesvd_(char* jobu, char* jobvt, integer* m, integer* n,
	real* a, integer* lda, real* s, real* u, integer* ldu, real* vt,
	integer* ldvt, real* work, integer* lwork, integer* info);

/* SVD of a general matrix by using divide and conquer algorithm - Used as standard SVD algorithm */
int sgesdd_(char* jobz, integer* m, integer* n, real* a,
	integer* lda, real* s, real* u, integer* ldu, real* vt, integer* ldvt,
	real* work, integer* lwork, integer* iwork, integer* info);

/* Cholesky factorization */
int spotrf_(char* uplo, integer* n, real* a, integer* lda, integer* info);

/* LUP factorizaton */
int sgetrf_(integer* m, integer* n, real* a, integer* lda, integer* ipiv, integer* info);

/* Solve AX = B using LUP factorization */
int sgetrs_(char* trans, integer* n, integer* nrhs, real* a, integer* lda, integer* ipiv, real* b, integer* ldb, integer* info);

/* Solve AX = B using Cholesky factorization */
int sposv_(char* uplo, integer* n, integer* nrhs, real* a, integer* lda, real* b, integer* ldb, integer* info);

/* Inverse */
int sgetri_(integer* n, real* a, integer* lda, integer* ipiv, real* work, integer* lwork, integer* info);

/* QR factorization */
int sgeqrf_(integer* m, integer* n, real* a, integer* lda, real* tau, real* work, integer* lwork, integer* info);
int sorgqr_(integer* m, integer* n, integer* k, real* a, integer* lda, real* tau, real* work, integer* lwork, integer* info);

/* Multiplication C = alpha*A*B + beta*C */
int sgemm_(char* transa, char* transb, integer* m, integer*
	n, integer* k, real* alpha, real* a, integer* lda, real* b, integer*
	ldb, real* beta, real* c__, integer* ldc);

/* Multiplication y = alpha*A*x + beta*y */
int sgemv_(char* trans, integer* m, integer* n, real* alpha,
	real* a, integer* lda, real* x, integer* incx, real* beta, real* y,
	integer* incy);

/* Multiplication y = alpha*A*x + beta*y where A is symmetric */
int ssymv_(char* uplo, integer* n, real* alpha, real* a,
	integer* lda, real* x, integer* incx, real* beta, real* y, integer*
	incy);

/* Multiplication C = alpha*A*B + beta*C * where A is symmetric */
int ssymm_(char* side, char* uplo, integer* m, integer* n,
	real* alpha, real* a, integer* lda, real* b, integer* ldb, real* beta,
	real* c__, integer* ldc);

#ifdef __cplusplus
}
#endif

#endif /* !ROUTINES */
