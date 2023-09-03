/* cosq1b.f -- translated by f2c (version 20100827).
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

static integer c__6 = 6;
static integer c__8 = 8;
static integer c__10 = 10;
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

/* Subroutine */ int cosq1b_(integer *n, integer *inc, real *x, integer *lenx,
	 real *wsave, integer *lensav, real *work, integer *lenwrk, integer *
	ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, i__1;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    static real x1;
    static integer ier1;
    extern /* Subroutine */ int cosqb1_(integer *, integer *, real *, real *, 
	    real *, integer *);
    static real ssqrt2;
    extern /* Subroutine */ int xerfft_(char *, integer *, ftnlen);


    /* Parameter adjustments */
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;

    if (*lenx < *inc * (*n - 1) + 1) {
	*ier = 1;
	xerfft_("COSQ1B", &c__6, (ftnlen)6);
	goto L300;
    } else if (*lensav < (*n << 1) + (integer) (log((real) (*n)) / log(2.f)) 
	    + 4) {
	*ier = 2;
	xerfft_("COSQ1B", &c__8, (ftnlen)6);
	goto L300;
    } else if (*lenwrk < *n) {
	*ier = 3;
	xerfft_("COSQ1B", &c__10, (ftnlen)6);
	goto L300;
    }

    if ((i__1 = *n - 2) < 0) {
	goto L300;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ssqrt2 = 1.f / sqrt(2.f);
    x1 = x[x_dim1 + 1] + x[(x_dim1 << 1) + 1];
    x[(x_dim1 << 1) + 1] = ssqrt2 * (x[x_dim1 + 1] - x[(x_dim1 << 1) + 1]);
    x[x_dim1 + 1] = x1;
    return 0;
L103:
    cosqb1_(n, inc, &x[x_offset], &wsave[1], &work[1], &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("COSQ1B", &c_n5, (ftnlen)6);
    }

L300:
    return 0;
} /* cosq1b_ */

