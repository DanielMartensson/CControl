/* mcsqb1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int mcsqb1_(integer *lot, integer *jump, integer *n, integer 
	*inc, real *x, real *wsave, real *work, integer *ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, work_dim1, work_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer i__, k, m, m1, kc, lj, np2, ns2, ier1;
    static real xim1;
    static integer modn, lenx, lnwk, lnsv;
    extern /* Subroutine */ int rfftmb_(integer *, integer *, integer *, 
	    integer *, real *, integer *, real *, integer *, real *, integer *
	    , integer *), xerfft_(char *, integer *, ftnlen);

    /* Parameter adjustments */
    work_dim1 = *lot;
    work_offset = 1 + work_dim1;
    work -= work_offset;
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;

    /* Function Body */
    *ier = 0;
    lj = (*lot - 1) * *jump + 1;
    ns2 = (*n + 1) / 2;
    np2 = *n + 2;
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	i__2 = lj;
	i__3 = *jump;
	for (m = 1; i__3 < 0 ? m >= i__2 : m <= i__2; m += i__3) {
	    xim1 = x[m + (i__ - 1) * x_dim1] + x[m + i__ * x_dim1];
	    x[m + i__ * x_dim1] = (x[m + (i__ - 1) * x_dim1] - x[m + i__ * 
		    x_dim1]) * .5f;
	    x[m + (i__ - 1) * x_dim1] = xim1 * .5f;
/* L201: */
	}
/* L101: */
    }
    i__1 = lj;
    i__3 = *jump;
    for (m = 1; i__3 < 0 ? m >= i__1 : m <= i__1; m += i__3) {
	x[m + x_dim1] *= .5f;
/* L301: */
    }
    modn = *n % 2;
    if (modn != 0) {
	goto L302;
    }
    i__3 = lj;
    i__1 = *jump;
    for (m = 1; i__1 < 0 ? m >= i__3 : m <= i__3; m += i__1) {
	x[m + *n * x_dim1] *= .5f;
/* L303: */
    }
L302:
    lenx = (*lot - 1) * *jump + *inc * (*n - 1) + 1;
    lnsv = *n + (integer) (log((real) (*n)) / log(2.f)) + 4;
    lnwk = *lot * *n;

    rfftmb_(lot, jump, n, inc, &x[x_offset], &lenx, &wsave[*n + 1], &lnsv, &
	    work[work_offset], &lnwk, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("MCSQB1", &c_n5, (ftnlen)6);
	goto L400;
    }

    i__1 = ns2;
    for (k = 2; k <= i__1; ++k) {
	kc = np2 - k;
	m1 = 0;
	i__3 = lj;
	i__2 = *jump;
	for (m = 1; i__2 < 0 ? m >= i__3 : m <= i__3; m += i__2) {
	    ++m1;
	    work[m1 + k * work_dim1] = wsave[k - 1] * x[m + kc * x_dim1] + 
		    wsave[kc - 1] * x[m + k * x_dim1];
	    work[m1 + kc * work_dim1] = wsave[k - 1] * x[m + k * x_dim1] - 
		    wsave[kc - 1] * x[m + kc * x_dim1];
/* L202: */
	}
/* L102: */
    }
    if (modn != 0) {
	goto L305;
    }
    i__1 = lj;
    i__2 = *jump;
    for (m = 1; i__2 < 0 ? m >= i__1 : m <= i__1; m += i__2) {
	x[m + (ns2 + 1) * x_dim1] = wsave[ns2] * (x[m + (ns2 + 1) * x_dim1] + 
		x[m + (ns2 + 1) * x_dim1]);
/* L304: */
    }
L305:
    i__2 = ns2;
    for (k = 2; k <= i__2; ++k) {
	kc = np2 - k;
	m1 = 0;
	i__1 = lj;
	i__3 = *jump;
	for (m = 1; i__3 < 0 ? m >= i__1 : m <= i__1; m += i__3) {
	    ++m1;
	    x[m + k * x_dim1] = work[m1 + k * work_dim1] + work[m1 + kc * 
		    work_dim1];
	    x[m + kc * x_dim1] = work[m1 + k * work_dim1] - work[m1 + kc * 
		    work_dim1];
/* L203: */
	}
/* L103: */
    }
    i__2 = lj;
    i__3 = *jump;
    for (m = 1; i__3 < 0 ? m >= i__2 : m <= i__2; m += i__3) {
	x[m + x_dim1] += x[m + x_dim1];
/* L104: */
    }
L400:
    return 0;
} /* mcsqb1_ */

