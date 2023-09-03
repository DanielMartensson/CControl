/* mradb3.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int mradb3_(integer *m, integer *ido, integer *l1, real *cc, 
	integer *im1, integer *in1, real *ch, integer *im2, integer *in2, 
	real *wa1, real *wa2)
{
    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_offset, ch_dim1, ch_dim2, ch_dim3, ch_offset,
	     i__1, i__2, i__3, i__4;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, k, m1, m2, ic, m1d, m2s;
    static real arg;
    static integer idp2;
    static real taui, taur;


    /* Parameter adjustments */
    --wa2;
    --wa1;
    cc_dim1 = *in1;
    cc_dim2 = *ido;
    cc_offset = 1 + cc_dim1 * (1 + (cc_dim2 << 2));
    cc -= cc_offset;
    ch_dim1 = *in2;
    ch_dim2 = *ido;
    ch_dim3 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2 * (1 + ch_dim3));
    ch -= ch_offset;

    /* Function Body */
    m1d = (*m - 1) * *im1 + 1;
    m2s = 1 - *im2;
    arg = atan(1.f) * 8.f / 3.f;
    taur = cos(arg);
    taui = sin(arg);
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	m2 = m2s;
	i__2 = m1d;
	i__3 = *im1;
	for (m1 = 1; i__3 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__3) {
	    m2 += *im2;
	    ch[m2 + ((k + ch_dim3) * ch_dim2 + 1) * ch_dim1] = cc[m1 + ((k * 
		    3 + 1) * cc_dim2 + 1) * cc_dim1] + cc[m1 + (*ido + (k * 3 
		    + 2) * cc_dim2) * cc_dim1] * 2.f;
	    ch[m2 + ((k + (ch_dim3 << 1)) * ch_dim2 + 1) * ch_dim1] = cc[m1 + 
		    ((k * 3 + 1) * cc_dim2 + 1) * cc_dim1] + taur * 2.f * cc[
		    m1 + (*ido + (k * 3 + 2) * cc_dim2) * cc_dim1] - taui * 
		    2.f * cc[m1 + ((k * 3 + 3) * cc_dim2 + 1) * cc_dim1];
	    ch[m2 + ((k + ch_dim3 * 3) * ch_dim2 + 1) * ch_dim1] = cc[m1 + ((
		    k * 3 + 1) * cc_dim2 + 1) * cc_dim1] + taur * 2.f * cc[m1 
		    + (*ido + (k * 3 + 2) * cc_dim2) * cc_dim1] + taui * 2.f *
		     cc[m1 + ((k * 3 + 3) * cc_dim2 + 1) * cc_dim1];
/* L1001: */
	}
/* L101: */
    }
    if (*ido == 1) {
	return 0;
    }
    idp2 = *ido + 2;
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__3 = *ido;
	for (i__ = 3; i__ <= i__3; i__ += 2) {
	    ic = idp2 - i__;
	    m2 = m2s;
	    i__2 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__4) {
		m2 += *im2;
		ch[m2 + (i__ - 1 + (k + ch_dim3) * ch_dim2) * ch_dim1] = cc[
			m1 + (i__ - 1 + (k * 3 + 1) * cc_dim2) * cc_dim1] + (
			cc[m1 + (i__ - 1 + (k * 3 + 3) * cc_dim2) * cc_dim1] 
			+ cc[m1 + (ic - 1 + (k * 3 + 2) * cc_dim2) * cc_dim1])
			;
		ch[m2 + (i__ + (k + ch_dim3) * ch_dim2) * ch_dim1] = cc[m1 + (
			i__ + (k * 3 + 1) * cc_dim2) * cc_dim1] + (cc[m1 + (
			i__ + (k * 3 + 3) * cc_dim2) * cc_dim1] - cc[m1 + (ic 
			+ (k * 3 + 2) * cc_dim2) * cc_dim1]);
		ch[m2 + (i__ - 1 + (k + (ch_dim3 << 1)) * ch_dim2) * ch_dim1] 
			= wa1[i__ - 2] * (cc[m1 + (i__ - 1 + (k * 3 + 1) * 
			cc_dim2) * cc_dim1] + taur * (cc[m1 + (i__ - 1 + (k * 
			3 + 3) * cc_dim2) * cc_dim1] + cc[m1 + (ic - 1 + (k * 
			3 + 2) * cc_dim2) * cc_dim1]) - taui * (cc[m1 + (i__ 
			+ (k * 3 + 3) * cc_dim2) * cc_dim1] + cc[m1 + (ic + (
			k * 3 + 2) * cc_dim2) * cc_dim1])) - wa1[i__ - 1] * (
			cc[m1 + (i__ + (k * 3 + 1) * cc_dim2) * cc_dim1] + 
			taur * (cc[m1 + (i__ + (k * 3 + 3) * cc_dim2) * 
			cc_dim1] - cc[m1 + (ic + (k * 3 + 2) * cc_dim2) * 
			cc_dim1]) + taui * (cc[m1 + (i__ - 1 + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] - cc[m1 + (ic - 1 + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]));
		ch[m2 + (i__ + (k + (ch_dim3 << 1)) * ch_dim2) * ch_dim1] = 
			wa1[i__ - 2] * (cc[m1 + (i__ + (k * 3 + 1) * cc_dim2) 
			* cc_dim1] + taur * (cc[m1 + (i__ + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] - cc[m1 + (ic + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]) + taui * (cc[m1 + (i__ - 1 + (k *
			 3 + 3) * cc_dim2) * cc_dim1] - cc[m1 + (ic - 1 + (k *
			 3 + 2) * cc_dim2) * cc_dim1])) + wa1[i__ - 1] * (cc[
			m1 + (i__ - 1 + (k * 3 + 1) * cc_dim2) * cc_dim1] + 
			taur * (cc[m1 + (i__ - 1 + (k * 3 + 3) * cc_dim2) * 
			cc_dim1] + cc[m1 + (ic - 1 + (k * 3 + 2) * cc_dim2) * 
			cc_dim1]) - taui * (cc[m1 + (i__ + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] + cc[m1 + (ic + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]));
		ch[m2 + (i__ - 1 + (k + ch_dim3 * 3) * ch_dim2) * ch_dim1] = 
			wa2[i__ - 2] * (cc[m1 + (i__ - 1 + (k * 3 + 1) * 
			cc_dim2) * cc_dim1] + taur * (cc[m1 + (i__ - 1 + (k * 
			3 + 3) * cc_dim2) * cc_dim1] + cc[m1 + (ic - 1 + (k * 
			3 + 2) * cc_dim2) * cc_dim1]) + taui * (cc[m1 + (i__ 
			+ (k * 3 + 3) * cc_dim2) * cc_dim1] + cc[m1 + (ic + (
			k * 3 + 2) * cc_dim2) * cc_dim1])) - wa2[i__ - 1] * (
			cc[m1 + (i__ + (k * 3 + 1) * cc_dim2) * cc_dim1] + 
			taur * (cc[m1 + (i__ + (k * 3 + 3) * cc_dim2) * 
			cc_dim1] - cc[m1 + (ic + (k * 3 + 2) * cc_dim2) * 
			cc_dim1]) - taui * (cc[m1 + (i__ - 1 + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] - cc[m1 + (ic - 1 + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]));
		ch[m2 + (i__ + (k + ch_dim3 * 3) * ch_dim2) * ch_dim1] = wa2[
			i__ - 2] * (cc[m1 + (i__ + (k * 3 + 1) * cc_dim2) * 
			cc_dim1] + taur * (cc[m1 + (i__ + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] - cc[m1 + (ic + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]) - taui * (cc[m1 + (i__ - 1 + (k *
			 3 + 3) * cc_dim2) * cc_dim1] - cc[m1 + (ic - 1 + (k *
			 3 + 2) * cc_dim2) * cc_dim1])) + wa2[i__ - 1] * (cc[
			m1 + (i__ - 1 + (k * 3 + 1) * cc_dim2) * cc_dim1] + 
			taur * (cc[m1 + (i__ - 1 + (k * 3 + 3) * cc_dim2) * 
			cc_dim1] + cc[m1 + (ic - 1 + (k * 3 + 2) * cc_dim2) * 
			cc_dim1]) + taui * (cc[m1 + (i__ + (k * 3 + 3) * 
			cc_dim2) * cc_dim1] + cc[m1 + (ic + (k * 3 + 2) * 
			cc_dim2) * cc_dim1]));
/* L1002: */
	    }
/* L102: */
	}
/* L103: */
    }
    return 0;
} /* mradb3_ */

