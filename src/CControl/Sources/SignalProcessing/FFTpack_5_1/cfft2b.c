/* cfft2b.f -- translated by f2c (version 20100827).
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

/* Table of constant values */

static integer c_n2 = -2;
static integer c__6 = 6;
static integer c__8 = 8;
static integer c__1 = 1;
static integer c_n5 = -5;

/*     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*     *                                                               * */
/*     *                  copyright (c) 2011 by UCAR                   * */
/*     *                                                               * */
/*     *       University Corporation for Atmospheric Research         * */
/*     *                                                               * */
/*     *                      all rights reserved                      * */
/*     *                                                               * */
/*     *                     FFTPACK  version 5.1                      * */
/*     *                                                               * */
/*     *                 A Fortran Package of Fast Fourier             * */
/*     *                                                               * */
/*     *                Subroutines and Example Programs               * */
/*     *                                                               * */
/*     *                             by                                * */
/*     *                                                               * */
/*     *               Paul Swarztrauber and Dick Valent               * */
/*     *                                                               * */
/*     *                             of                                * */
/*     *                                                               * */
/*     *         the National Center for Atmospheric Research          * */
/*     *                                                               * */
/*     *                Boulder, Colorado  (80307)  U.S.A.             * */
/*     *                                                               * */
/*     *                   which is sponsored by                       * */
/*     *                                                               * */
/*     *              the National Science Foundation                  * */
/*     *                                                               * */
/*     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Subroutine */ int cfft2b_(integer *ldim, integer *l, integer *m, complex *
	c__, real *wsave, integer *lensav, real *work, integer *lenwrk, 
	integer *ier)
{
    /* System generated locals */
    integer c_dim1, c_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer iw, ier1;
    extern /* Subroutine */ int cfftmb_(integer *, integer *, integer *, 
	    integer *, complex *, integer *, real *, integer *, real *, 
	    integer *, integer *), xerfft_(char *, integer *, ftnlen);


/* Initialize error return */

    /* Parameter adjustments */
    c_dim1 = *ldim;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;

    if (*l > *ldim) {
	*ier = 5;
	xerfft_("CFFT2B", &c_n2, (ftnlen)6);
	goto L100;
    } else if (*lensav < (*l << 1) + (integer) (log((real) (*l)) / log(2.f)) 
	    + (*m << 1) + (integer) (log((real) (*m)) / log(2.f)) + 8) {
	*ier = 2;
	xerfft_("CFFT2B", &c__6, (ftnlen)6);
	goto L100;
    } else if (*lenwrk < (*l << 1) * *m) {
	*ier = 3;
	xerfft_("CFFT2B", &c__8, (ftnlen)6);
	goto L100;
    }

/* Transform X lines of C array */
    iw = (*l << 1) + (integer) (log((real) (*l)) / log(2.f)) + 3;
    i__1 = *l - 1 + *ldim * (*m - 1) + 1;
    i__2 = (*m << 1) + (integer) (log((real) (*m)) / log(2.f)) + 4;
    i__3 = (*l << 1) * *m;
    cfftmb_(l, &c__1, m, ldim, &c__[c_offset], &i__1, &wsave[iw], &i__2, &
	    work[1], &i__3, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("CFFT2B", &c_n5, (ftnlen)6);
	goto L100;
    }

/* Transform Y lines of C array */
    iw = 1;
    i__1 = (*m - 1) * *ldim + *l;
    i__2 = (*l << 1) + (integer) (log((real) (*l)) / log(2.f)) + 4;
    i__3 = (*m << 1) * *l;
    cfftmb_(m, ldim, l, &c__1, &c__[c_offset], &i__1, &wsave[iw], &i__2, &
	    work[1], &i__3, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("CFFT2B", &c_n5, (ftnlen)6);
    }

L100:
    return 0;
} /* cfft2b_ */

