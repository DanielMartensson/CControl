/* cosqf1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cosqf1_(integer *n, integer *inc, real *x, real *wsave, 
	real *work, integer *ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, i__1;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer i__, k, kc, np2, ns2, ier1;
    static real xim1;
    static integer modn, lenx, lnwk, lnsv;
    extern /* Subroutine */ int rfft1f_(integer *, integer *, real *, integer 
	    *, real *, integer *, real *, integer *, integer *), xerfft_(char 
	    *, integer *, ftnlen);

    /* Parameter adjustments */
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;
    ns2 = (*n + 1) / 2;
    np2 = *n + 2;
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	work[k] = x[k * x_dim1 + 1] + x[kc * x_dim1 + 1];
	work[kc] = x[k * x_dim1 + 1] - x[kc * x_dim1 + 1];
/* L101: */
    }
    modn = *n % 2;
    if (modn != 0) {
	goto L301;
    }
    work[ns2 + 1] = x[(ns2 + 1) * x_dim1 + 1] + x[(ns2 + 1) * x_dim1 + 1];
L301:
    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	x[k * x_dim1 + 1] = wsave[k - 1] * work[kc] + wsave[kc - 1] * work[k];
	x[kc * x_dim1 + 1] = wsave[k - 1] * work[k] - wsave[kc - 1] * work[kc]
		;
/* L102: */
    }
    if (modn != 0) {
	goto L303;
    }
    x[(ns2 + 1) * x_dim1 + 1] = wsave[ns2] * work[ns2 + 1];
L303:
    lenx = *inc * (*n - 1) + 1;
    lnsv = *n + (integer) (log((real) (*n)) / log(2.f)) + 4;
    lnwk = *n;

    rfft1f_(n, inc, &x[x_offset], &lenx, &wsave[*n + 1], &lnsv, &work[1], &
	    lnwk, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("COSQF1", &c_n5, (ftnlen)6);
	goto L400;
    }

    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	xim1 = (x[(i__ - 1) * x_dim1 + 1] + x[i__ * x_dim1 + 1]) * .5f;
	x[i__ * x_dim1 + 1] = (x[(i__ - 1) * x_dim1 + 1] - x[i__ * x_dim1 + 1]
		) * .5f;
	x[(i__ - 1) * x_dim1 + 1] = xim1;
/* L103: */
    }
L400:
    return 0;
} /* cosqf1_ */

