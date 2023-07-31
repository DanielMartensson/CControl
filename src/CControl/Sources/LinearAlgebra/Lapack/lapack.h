#ifndef LAPACK
#define LAPACK

#include "f2c.h"

int ssyevd_(char* jobz, char* uplo, integer* n, real* a,
	integer* lda, real* w, real* work, integer* lwork, integer* iwork,
	integer* liwork, integer* info);

int dsyevd_(char* jobz, char* uplo, integer* n, doublereal*
	a, integer* lda, doublereal* w, doublereal* work, integer* lwork,
	integer* iwork, integer* liwork, integer* info);

#endif // !LAPACK
