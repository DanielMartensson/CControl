/* mcstf1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int mcstf1_(integer *lot, integer *jump, integer *n, integer 
	*inc, real *x, real *wsave, doublereal *dsum, real *work, integer *
	ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer i__, k, m, m1;
    static real t1, t2;
    static integer kc, lj;
    static real xi;
    static integer nm1, np1;
    static real x1h;
    static integer ns2;
    static real tx2, x1p3;
    static integer ier1;
    static real snm1;
    static integer modn, lenx, lnwk, lnsv;
    extern /* Subroutine */ int rfftmf_(integer *, integer *, integer *, 
	    integer *, real *, integer *, real *, integer *, real *, integer *
	    , integer *), xerfft_(char *, integer *, ftnlen);

    /* Parameter adjustments */
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;
    --dsum;

    /* Function Body */
    *ier = 0;
    nm1 = *n - 1;
    np1 = *n + 1;
    ns2 = *n / 2;
    lj = (*lot - 1) * *jump + 1;
    if ((i__1 = *n - 2) < 0) {
	goto L200;
    } else if (i__1 == 0) {
	goto L101;
    } else {
	goto L102;
    }
L101:
    i__1 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
	x1h = x[m + x_dim1] + x[m + (x_dim1 << 1)];
	x[m + (x_dim1 << 1)] = (x[m + x_dim1] - x[m + (x_dim1 << 1)]) * .5f;
	x[m + x_dim1] = x1h * .5f;
/* L111: */
    }
    goto L200;
L102:
    if (*n > 3) {
	goto L103;
    }
    i__2 = lj;
    i__1 = *jump;
    for (m = 1; i__1 < 0 ? m >= i__2 : m <= i__2; m += i__1) {
	x1p3 = x[m + x_dim1] + x[m + x_dim1 * 3];
	tx2 = x[m + (x_dim1 << 1)] + x[m + (x_dim1 << 1)];
	x[m + (x_dim1 << 1)] = (x[m + x_dim1] - x[m + x_dim1 * 3]) * .5f;
	x[m + x_dim1] = (x1p3 + tx2) * .25f;
	x[m + x_dim1 * 3] = (x1p3 - tx2) * .25f;
/* L112: */
    }
    goto L200;
L103:
    m1 = 0;
    i__1 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
	++m1;
	dsum[m1] = x[m + x_dim1] - x[m + *n * x_dim1];
	x[m + x_dim1] += x[m + *n * x_dim1];
/* L113: */
    }
    i__2 = ns2;
    for (k = 2; k <= i__2; ++k) {
	m1 = 0;
	i__1 = lj;
	i__3 = *jump;
	for (m = 1; i__3 < 0 ? m >= i__1 : m <= i__1; m += i__3) {
	    ++m1;
	    kc = np1 - k;
	    t1 = x[m + k * x_dim1] + x[m + kc * x_dim1];
	    t2 = x[m + k * x_dim1] - x[m + kc * x_dim1];
	    dsum[m1] += wsave[kc] * t2;
	    t2 = wsave[k] * t2;
	    x[m + k * x_dim1] = t1 - t2;
	    x[m + kc * x_dim1] = t1 + t2;
/* L114: */
	}
/* L104: */
    }
    modn = *n % 2;
    if (modn == 0) {
	goto L124;
    }
    i__2 = lj;
    i__3 = *jump;
    for (m = 1; i__3 < 0 ? m >= i__2 : m <= i__2; m += i__3) {
	x[m + (ns2 + 1) * x_dim1] += x[m + (ns2 + 1) * x_dim1];
/* L123: */
    }
L124:
    lenx = (*lot - 1) * *jump + *inc * (nm1 - 1) + 1;
    lnsv = nm1 + (integer) (log((real) nm1) / log(2.f)) + 4;
    lnwk = *lot * nm1;

    rfftmf_(lot, jump, &nm1, inc, &x[x_offset], &lenx, &wsave[*n + 1], &lnsv, 
	    work, &lnwk, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("MCSTF1", &c_n5, (ftnlen)6);
	goto L200;
    }

    snm1 = 1.f / (real) nm1;
    i__3 = *lot;
    for (m = 1; m <= i__3; ++m) {
	dsum[m] = snm1 * dsum[m];
/* L10: */
    }
    if (nm1 % 2 != 0) {
	goto L30;
    }
    i__3 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__3 : m <= i__3; m += i__2) {
	x[m + nm1 * x_dim1] += x[m + nm1 * x_dim1];
/* L20: */
    }
L30:
    i__2 = *n;
    for (i__ = 3; i__ <= i__2; i__ += 2) {
	m1 = 0;
	i__3 = lj;
	i__1 = *jump;
	for (m = 1; i__1 < 0 ? m >= i__3 : m <= i__3; m += i__1) {
	    ++m1;
	    xi = x[m + i__ * x_dim1] * .5f;
	    x[m + i__ * x_dim1] = x[m + (i__ - 1) * x_dim1] * .5f;
	    x[m + (i__ - 1) * x_dim1] = dsum[m1];
	    dsum[m1] += xi;
/* L115: */
	}
/* L105: */
    }
    if (modn != 0) {
	goto L117;
    }
    m1 = 0;
    i__2 = lj;
    i__1 = *jump;
    for (m = 1; i__1 < 0 ? m >= i__2 : m <= i__2; m += i__1) {
	++m1;
	x[m + *n * x_dim1] = dsum[m1];
/* L116: */
    }
L117:
    i__1 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
	x[m + x_dim1] *= .5f;
	x[m + *n * x_dim1] *= .5f;
/* L118: */
    }

L200:
    return 0;
} /* mcstf1_ */

