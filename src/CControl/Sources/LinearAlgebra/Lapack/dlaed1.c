/* dlaed1.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static integer c__1 = 1;
static integer c_n1 = -1;

/* Subroutine */ int dlaed1_(integer *n, doublereal *d__, doublereal *q, 
	integer *ldq, integer *indxq, doublereal *rho, integer *cutpnt, 
	doublereal *work, integer *iwork, integer *info)
{
    /* System generated locals */
    integer q_dim1, q_offset, i__1, i__2;

    /* Local variables */
    integer i__, k, n1, n2, is, iw, iz, iq2, zpp1, indx, indxc;
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *, 
	    doublereal *, integer *);
    integer indxp;
    extern /* Subroutine */ int dlaed2_(integer *, integer *, integer *, 
	    doublereal *, doublereal *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *, 
	     integer *, integer *, integer *, integer *), dlaed3_(integer *, 
	    integer *, integer *, doublereal *, doublereal *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, integer *);
    integer idlmda;
    extern /* Subroutine */ int dlamrg_(integer *, integer *, doublereal *, 
	    integer *, integer *, integer *), xerbla_(char *, integer *);
    integer coltyp;


/*  -- LAPACK routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DLAED1 computes the updated eigensystem of a diagonal */
/*  matrix after modification by a rank-one symmetric matrix.  This */
/*  routine is used only for the eigenproblem which requires all */
/*  eigenvalues and eigenvectors of a tridiagonal matrix.  DLAED7 handles */
/*  the case in which eigenvalues only or eigenvalues and eigenvectors */
/*  of a full symmetric matrix (which was reduced to tridiagonal form) */
/*  are desired. */

/*    T = Q(in) ( D(in) + RHO * Z*Z' ) Q'(in) = Q(out) * D(out) * Q'(out) */

/*     where Z = Q'u, u is a vector of length N with ones in the */
/*     CUTPNT and CUTPNT + 1 th elements and zeros elsewhere. */

/*     The eigenvectors of the original matrix are stored in Q, and the */
/*     eigenvalues are in D.  The algorithm consists of three stages: */

/*        The first stage consists of deflating the size of the problem */
/*        when there are multiple eigenvalues or if there is a zero in */
/*        the Z vector.  For each such occurence the dimension of the */
/*        secular equation problem is reduced by one.  This stage is */
/*        performed by the routine DLAED2. */

/*        The second stage consists of calculating the updated */
/*        eigenvalues. This is done by finding the roots of the secular */
/*        equation via the routine DLAED4 (as called by DLAED3). */
/*        This routine also calculates the eigenvectors of the current */
/*        problem. */

/*        The final stage consists of computing the updated eigenvectors */
/*        directly using the updated eigenvalues.  The eigenvectors for */
/*        the current problem are multiplied with the eigenvectors from */
/*        the overall problem. */

/*  Arguments */
/*  ========= */

/*  N      (input) INTEGER */
/*         The dimension of the symmetric tridiagonal matrix.  N >= 0. */

/*  D      (input/output) DOUBLE PRECISION array, dimension (N) */
/*         On entry, the eigenvalues of the rank-1-perturbed matrix. */
/*         On exit, the eigenvalues of the repaired matrix. */

/*  Q      (input/output) DOUBLE PRECISION array, dimension (LDQ,N) */
/*         On entry, the eigenvectors of the rank-1-perturbed matrix. */
/*         On exit, the eigenvectors of the repaired tridiagonal matrix. */

/*  LDQ    (input) INTEGER */
/*         The leading dimension of the array Q.  LDQ >= max(1,N). */

/*  INDXQ  (input/output) INTEGER array, dimension (N) */
/*         On entry, the permutation which separately sorts the two */
/*         subproblems in D into ascending order. */
/*         On exit, the permutation which will reintegrate the */
/*         subproblems back into sorted order, */
/*         i.e. D( INDXQ( I = 1, N ) ) will be in ascending order. */

/*  RHO    (input) DOUBLE PRECISION */
/*         The subdiagonal entry used to create the rank-1 modification. */

/*  CUTPNT (input) INTEGER */
/*         The location of the last eigenvalue in the leading sub-matrix. */
/*         min(1,N) <= CUTPNT <= N/2. */

/*  WORK   (workspace) DOUBLE PRECISION array, dimension (4*N + N**2) */

/*  IWORK  (workspace) INTEGER array, dimension (4*N) */

/*  INFO   (output) INTEGER */
/*          = 0:  successful exit. */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = 1, an eigenvalue did not converge */

/*  Further Details */
/*  =============== */

/*  Based on contributions by */
/*     Jeff Rutter, Computer Science Division, University of California */
/*     at Berkeley, USA */
/*  Modified by Francoise Tisseur, University of Tennessee. */

/*  ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --d__;
    q_dim1 = *ldq;
    q_offset = 1 + q_dim1;
    q -= q_offset;
    --indxq;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;

    if (*n < 0) {
	*info = -1;
    } else if (*ldq < max(1,*n)) {
	*info = -4;
    } else /* if(complicated condition) */ {
/* Computing MIN */
	i__1 = 1, i__2 = *n / 2;
	if (min(i__1,i__2) > *cutpnt || *n / 2 < *cutpnt) {
	    *info = -7;
	}
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DLAED1", &i__1);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

/*     The following values are integer pointers which indicate */
/*     the portion of the workspace */
/*     used by a particular array in DLAED2 and DLAED3. */

    iz = 1;
    idlmda = iz + *n;
    iw = idlmda + *n;
    iq2 = iw + *n;

    indx = 1;
    indxc = indx + *n;
    coltyp = indxc + *n;
    indxp = coltyp + *n;


/*     Form the z-vector which consists of the last row of Q_1 and the */
/*     first row of Q_2. */

    dcopy_(cutpnt, &q[*cutpnt + q_dim1], ldq, &work[iz], &c__1);
    zpp1 = *cutpnt + 1;
    i__1 = *n - *cutpnt;
    dcopy_(&i__1, &q[zpp1 + zpp1 * q_dim1], ldq, &work[iz + *cutpnt], &c__1);

/*     Deflate eigenvalues. */

    dlaed2_(&k, n, cutpnt, &d__[1], &q[q_offset], ldq, &indxq[1], rho, &work[
	    iz], &work[idlmda], &work[iw], &work[iq2], &iwork[indx], &iwork[
	    indxc], &iwork[indxp], &iwork[coltyp], info);

    if (*info != 0) {
	goto L20;
    }

/*     Solve Secular Equation. */

    if (k != 0) {
	is = (iwork[coltyp] + iwork[coltyp + 1]) * *cutpnt + (iwork[coltyp + 
		1] + iwork[coltyp + 2]) * (*n - *cutpnt) + iq2;
	dlaed3_(&k, n, cutpnt, &d__[1], &q[q_offset], ldq, rho, &work[idlmda], 
		 &work[iq2], &iwork[indxc], &iwork[coltyp], &work[iw], &work[
		is], info);
	if (*info != 0) {
	    goto L20;
	}

/*     Prepare the INDXQ sorting permutation. */

	n1 = k;
	n2 = *n - k;
	dlamrg_(&n1, &n2, &d__[1], &c__1, &c_n1, &indxq[1]);
    } else {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    indxq[i__] = i__;
/* L10: */
	}
    }

L20:
    return 0;

/*     End of DLAED1 */

} /* dlaed1_ */
