/* msntf1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int msntf1_(integer *lot, integer *jump, integer *n, integer 
	*inc, real *x, real *wsave, doublereal *dsum, real *xh, real *work, 
	integer *ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, xh_dim1, xh_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double sqrt(doublereal), log(doublereal);

    /* Local variables */
    static integer i__, k, m, m1;
    static real t1, t2;
    static integer kc, lj, np1, ns2, ier1, modn, lnxh, lnwk, lnsv;
    static real sfnp1, xhold, ssqrt3;
    extern /* Subroutine */ int rfftmf_(integer *, integer *, integer *, 
	    integer *, real *, integer *, real *, integer *, real *, integer *
	    , integer *), xerfft_(char *, integer *, ftnlen);

    /* Parameter adjustments */
    xh_dim1 = *lot;
    xh_offset = 1 + xh_dim1;
    xh -= xh_offset;
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;
    --dsum;

    /* Function Body */
    *ier = 0;
    lj = (*lot - 1) * *jump + 1;
    if ((i__1 = *n - 2) < 0) {
	goto L101;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    ssqrt3 = 1.f / sqrt(3.f);
    i__1 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
	xhold = ssqrt3 * (x[m + x_dim1] + x[m + (x_dim1 << 1)]);
	x[m + (x_dim1 << 1)] = ssqrt3 * (x[m + x_dim1] - x[m + (x_dim1 << 1)])
		;
	x[m + x_dim1] = xhold;
/* L112: */
    }
L101:
    goto L200;
L103:
    np1 = *n + 1;
    ns2 = *n / 2;
    i__2 = ns2;
    for (k = 1; k <= i__2; ++k) {
	kc = np1 - k;
	m1 = 0;
	i__1 = lj;
	i__3 = *jump;
	for (m = 1; i__3 < 0 ? m >= i__1 : m <= i__1; m += i__3) {
	    ++m1;
	    t1 = x[m + k * x_dim1] - x[m + kc * x_dim1];
	    t2 = wsave[k] * (x[m + k * x_dim1] + x[m + kc * x_dim1]);
	    xh[m1 + (k + 1) * xh_dim1] = t1 + t2;
	    xh[m1 + (kc + 1) * xh_dim1] = t2 - t1;
/* L114: */
	}
/* L104: */
    }
    modn = *n % 2;
    if (modn == 0) {
	goto L124;
    }
    m1 = 0;
    i__2 = lj;
    i__3 = *jump;
    for (m = 1; i__3 < 0 ? m >= i__2 : m <= i__2; m += i__3) {
	++m1;
	xh[m1 + (ns2 + 2) * xh_dim1] = x[m + (ns2 + 1) * x_dim1] * 4.f;
/* L123: */
    }
L124:
    i__3 = *lot;
    for (m = 1; m <= i__3; ++m) {
	xh[m + xh_dim1] = 0.f;
/* L127: */
    }
    lnxh = *lot - 1 + *lot * (np1 - 1) + 1;
    lnsv = np1 + (integer) (log((real) np1) / log(2.f)) + 4;
    lnwk = *lot * np1;

    rfftmf_(lot, &c__1, &np1, lot, &xh[xh_offset], &lnxh, &wsave[ns2 + 1], &
	    lnsv, work, &lnwk, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("MSNTF1", &c_n5, (ftnlen)6);
	goto L200;
    }

    if (np1 % 2 != 0) {
	goto L30;
    }
    i__3 = *lot;
    for (m = 1; m <= i__3; ++m) {
	xh[m + np1 * xh_dim1] += xh[m + np1 * xh_dim1];
/* L20: */
    }
L30:
    sfnp1 = 1.f / (real) np1;
    m1 = 0;
    i__3 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__3 : m <= i__3; m += i__2) {
	++m1;
	x[m + x_dim1] = xh[m1 + xh_dim1] * .5f;
	dsum[m1] = x[m + x_dim1];
/* L125: */
    }
    i__2 = *n;
    for (i__ = 3; i__ <= i__2; i__ += 2) {
	m1 = 0;
	i__3 = lj;
	i__1 = *jump;
	for (m = 1; i__1 < 0 ? m >= i__3 : m <= i__3; m += i__1) {
	    ++m1;
	    x[m + (i__ - 1) * x_dim1] = xh[m1 + i__ * xh_dim1] * .5f;
	    dsum[m1] += xh[m1 + (i__ - 1) * xh_dim1] * .5f;
	    x[m + i__ * x_dim1] = dsum[m1];
/* L115: */
	}
/* L105: */
    }
    if (modn != 0) {
	goto L200;
    }
    m1 = 0;
    i__2 = lj;
    i__1 = *jump;
    for (m = 1; i__1 < 0 ? m >= i__2 : m <= i__2; m += i__1) {
	++m1;
	x[m + *n * x_dim1] = xh[m1 + (*n + 1) * xh_dim1] * .5f;
/* L116: */
    }
L200:
    return 0;
} /* msntf1_ */

