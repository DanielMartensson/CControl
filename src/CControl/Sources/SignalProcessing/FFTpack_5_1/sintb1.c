/* sintb1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int sintb1_(integer *n, integer *inc, real *x, real *wsave, 
	real *xh, real *work, integer *ier)
{
    /* System generated locals */
    integer x_dim1, x_offset, i__1;

    /* Builtin functions */
    double sqrt(doublereal), log(doublereal);

    /* Local variables */
    static integer i__, k;
    static real t1, t2;
    static integer kc, np1, ns2, ier1, modn;
    static doublereal dsum;
    static integer lnxh, lnwk, lnsv;
    static real fnp1s4, xhold, srt3s2;
    extern /* Subroutine */ int rfft1f_(integer *, integer *, real *, integer 
	    *, real *, integer *, real *, integer *, integer *), xerfft_(char 
	    *, integer *, ftnlen);

    /* Parameter adjustments */
    x_dim1 = *inc;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --wsave;
    --xh;

    /* Function Body */
    *ier = 0;
    if ((i__1 = *n - 2) < 0) {
	goto L200;
    } else if (i__1 == 0) {
	goto L102;
    } else {
	goto L103;
    }
L102:
    srt3s2 = sqrt(3.f) / 2.f;
    xhold = srt3s2 * (x[x_dim1 + 1] + x[(x_dim1 << 1) + 1]);
    x[(x_dim1 << 1) + 1] = srt3s2 * (x[x_dim1 + 1] - x[(x_dim1 << 1) + 1]);
    x[x_dim1 + 1] = xhold;
    goto L200;
L103:
    np1 = *n + 1;
    ns2 = *n / 2;
    i__1 = ns2;
    for (k = 1; k <= i__1; ++k) {
	kc = np1 - k;
	t1 = x[k * x_dim1 + 1] - x[kc * x_dim1 + 1];
	t2 = wsave[k] * (x[k * x_dim1 + 1] + x[kc * x_dim1 + 1]);
	xh[k + 1] = t1 + t2;
	xh[kc + 1] = t2 - t1;
/* L104: */
    }
    modn = *n % 2;
    if (modn == 0) {
	goto L124;
    }
    xh[ns2 + 2] = x[(ns2 + 1) * x_dim1 + 1] * 4.f;
L124:
    xh[1] = 0.f;
    lnxh = np1;
    lnsv = np1 + (integer) (log((real) np1) / log(2.f)) + 4;
    lnwk = np1;

    rfft1f_(&np1, &c__1, &xh[1], &lnxh, &wsave[ns2 + 1], &lnsv, work, &lnwk, &
	    ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("SINTB1", &c_n5, (ftnlen)6);
	goto L200;
    }

    if (np1 % 2 != 0) {
	goto L30;
    }
    xh[np1] += xh[np1];
L30:
    fnp1s4 = (real) np1 / 4.f;
    x[x_dim1 + 1] = fnp1s4 * xh[1];
    dsum = x[x_dim1 + 1];
    i__1 = *n;
    for (i__ = 3; i__ <= i__1; i__ += 2) {
	x[(i__ - 1) * x_dim1 + 1] = fnp1s4 * xh[i__];
	dsum += fnp1s4 * xh[i__ - 1];
	x[i__ * x_dim1 + 1] = dsum;
/* L105: */
    }
    if (modn != 0) {
	goto L200;
    }
    x[*n * x_dim1 + 1] = fnp1s4 * xh[*n + 1];

L200:
    return 0;
} /* sintb1_ */

