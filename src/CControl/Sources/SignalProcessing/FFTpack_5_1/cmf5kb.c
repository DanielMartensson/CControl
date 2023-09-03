/* cmf5kb.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cmf5kb_(integer *lot, integer *ido, integer *l1, integer 
	*na, real *cc, integer *im1, integer *in1, real *ch, integer *im2, 
	integer *in2, real *wa)
{
    /* Initialized data */

    static real tr11 = .3090169943749474f;
    static real ti11 = .9510565162951536f;
    static real tr12 = -.8090169943749474f;
    static real ti12 = .5877852522924731f;

    /* System generated locals */
    integer cc_dim2, cc_dim3, cc_dim4, cc_offset, ch_dim2, ch_dim3, ch_offset,
	     wa_dim1, wa_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, k, m1, m2;
    static real ci2, ci3, ci4, ci5;
    static integer m1d;
    static real di3, di4, di5, di2, cr2, cr3, cr5, cr4, ti2, ti3, ti4;
    static integer m2s;
    static real ti5, dr3, dr4, dr5, dr2, tr2, tr3, tr4, tr5, chold1, chold2;

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_offset = 1 + wa_dim1 * 5;
    wa -= wa_offset;
    cc_dim2 = *in1;
    cc_dim3 = *l1;
    cc_dim4 = *ido;
    cc_offset = 1 + 2 * (1 + cc_dim2 * (1 + cc_dim3 * (1 + cc_dim4)));
    cc -= cc_offset;
    ch_dim2 = *in2;
    ch_dim3 = *l1;
    ch_offset = 1 + 2 * (1 + ch_dim2 * (1 + ch_dim3 * 6));
    ch -= ch_offset;

    /* Function Body */

/* FFTPACK 5.0 auxiliary routine */

    m1d = (*lot - 1) * *im1 + 1;
    m2s = 1 - *im2;
    if (*ido > 1 || *na == 1) {
	goto L102;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = m1d;
	i__3 = *im1;
	for (m1 = 1; i__3 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__3) {
	    ti5 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] - cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] + cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti4 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 2] - cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti3 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 2] + cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    tr5 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] - cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] + cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr4 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 1] - cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr3 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 1] + cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    chold1 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 
		    1] + tr2 + tr3;
	    chold2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 
		    2] + ti2 + ti3;
	    cr2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + tr11 * tr2 + tr12 * tr3;
	    ci2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + tr11 * ti2 + tr12 * ti3;
	    cr3 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + tr12 * tr2 + tr11 * tr3;
	    ci3 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + tr12 * ti2 + tr11 * ti3;
	    cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] = 
		    chold1;
	    cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] = 
		    chold2;
	    cr5 = ti11 * tr5 + ti12 * tr4;
	    ci5 = ti11 * ti5 + ti12 * ti4;
	    cr4 = ti12 * tr5 - ti11 * tr4;
	    ci4 = ti12 * ti5 - ti11 * ti4;
	    cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 1) + 1]
		     = cr2 - ci5;
	    cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * cc_dim2 << 1) + 1] = 
		    cr2 + ci5;
	    cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 1) + 2]
		     = ci2 + cr5;
	    cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) + 2] = 
		    ci3 + cr4;
	    cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) + 1] = 
		    cr3 - ci4;
	    cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * cc_dim2 << 1) + 1]
		     = cr3 + ci4;
	    cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * cc_dim2 << 1) + 2]
		     = ci3 - cr4;
	    cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * cc_dim2 << 1) + 2] = 
		    ci2 - cr5;
/* L101: */
	}
    }
    return 0;
L102:
    i__3 = *l1;
    for (k = 1; k <= i__3; ++k) {
	m2 = m2s;
	i__2 = m1d;
	i__1 = *im1;
	for (m1 = 1; i__1 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__1) {
	    m2 += *im2;
	    ti5 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] - cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] + cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti4 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 2] - cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ti3 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 2] + cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    tr5 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] - cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] + cc[(m1 + (k + (cc_dim4 * 5 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr4 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 1] - cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    tr3 = cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 1] + cc[(m1 + (k + ((cc_dim4 << 2) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 1] = cc[(m1 + (k + (
		    cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] + tr2 + tr3;
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 2] = cc[(m1 + (k + (
		    cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] + ti2 + ti3;
	    cr2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + tr11 * tr2 + tr12 * tr3;
	    ci2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + tr11 * ti2 + tr12 * ti3;
	    cr3 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + tr12 * tr2 + tr11 * tr3;
	    ci3 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + tr12 * ti2 + tr11 * ti3;
	    cr5 = ti11 * tr5 + ti12 * tr4;
	    ci5 = ti11 * ti5 + ti12 * ti4;
	    cr4 = ti12 * tr5 - ti11 * tr4;
	    ci4 = ti12 * ti5 - ti11 * ti4;
	    ch[(m2 + (k + ch_dim3 * 7) * ch_dim2 << 1) + 1] = cr2 - ci5;
	    ch[(m2 + (k + ch_dim3 * 10) * ch_dim2 << 1) + 1] = cr2 + ci5;
	    ch[(m2 + (k + ch_dim3 * 7) * ch_dim2 << 1) + 2] = ci2 + cr5;
	    ch[(m2 + (k + (ch_dim3 << 3)) * ch_dim2 << 1) + 2] = ci3 + cr4;
	    ch[(m2 + (k + (ch_dim3 << 3)) * ch_dim2 << 1) + 1] = cr3 - ci4;
	    ch[(m2 + (k + ch_dim3 * 9) * ch_dim2 << 1) + 1] = cr3 + ci4;
	    ch[(m2 + (k + ch_dim3 * 9) * ch_dim2 << 1) + 2] = ci3 - cr4;
	    ch[(m2 + (k + ch_dim3 * 10) * ch_dim2 << 1) + 2] = ci2 - cr5;
/* L103: */
	}
    }
    if (*ido == 1) {
	return 0;
    }
    i__1 = *ido;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    m2 = m2s;
	    i__3 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__3 : m1 <= i__3; m1 += i__4) {
		m2 += *im2;
		ti5 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 2] - cc[(m1 + (k + (i__ + cc_dim4 * 5)
			 * cc_dim3) * cc_dim2 << 1) + 2];
		ti2 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 2] + cc[(m1 + (k + (i__ + cc_dim4 * 5)
			 * cc_dim3) * cc_dim2 << 1) + 2];
		ti4 = cc[(m1 + (k + (i__ + cc_dim4 * 3) * cc_dim3) * cc_dim2 
			<< 1) + 2] - cc[(m1 + (k + (i__ + (cc_dim4 << 2)) * 
			cc_dim3) * cc_dim2 << 1) + 2];
		ti3 = cc[(m1 + (k + (i__ + cc_dim4 * 3) * cc_dim3) * cc_dim2 
			<< 1) + 2] + cc[(m1 + (k + (i__ + (cc_dim4 << 2)) * 
			cc_dim3) * cc_dim2 << 1) + 2];
		tr5 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 1] - cc[(m1 + (k + (i__ + cc_dim4 * 5)
			 * cc_dim3) * cc_dim2 << 1) + 1];
		tr2 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 1] + cc[(m1 + (k + (i__ + cc_dim4 * 5)
			 * cc_dim3) * cc_dim2 << 1) + 1];
		tr4 = cc[(m1 + (k + (i__ + cc_dim4 * 3) * cc_dim3) * cc_dim2 
			<< 1) + 1] - cc[(m1 + (k + (i__ + (cc_dim4 << 2)) * 
			cc_dim3) * cc_dim2 << 1) + 1];
		tr3 = cc[(m1 + (k + (i__ + cc_dim4 * 3) * cc_dim3) * cc_dim2 
			<< 1) + 1] + cc[(m1 + (k + (i__ + (cc_dim4 << 2)) * 
			cc_dim3) * cc_dim2 << 1) + 1];
		ch[(m2 + (k + (i__ * 5 + 1) * ch_dim3) * ch_dim2 << 1) + 1] = 
			cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 
			1) + 1] + tr2 + tr3;
		ch[(m2 + (k + (i__ * 5 + 1) * ch_dim3) * ch_dim2 << 1) + 2] = 
			cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 
			1) + 2] + ti2 + ti3;
		cr2 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 1] + tr11 * tr2 + tr12 * tr3;
		ci2 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 2] + tr11 * ti2 + tr12 * ti3;
		cr3 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 1] + tr12 * tr2 + tr11 * tr3;
		ci3 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 2] + tr12 * ti2 + tr11 * ti3;
		cr5 = ti11 * tr5 + ti12 * tr4;
		ci5 = ti11 * ti5 + ti12 * ti4;
		cr4 = ti12 * tr5 - ti11 * tr4;
		ci4 = ti12 * ti5 - ti11 * ti4;
		dr3 = cr3 - ci4;
		dr4 = cr3 + ci4;
		di3 = ci3 + cr4;
		di4 = ci3 - cr4;
		dr5 = cr2 + ci5;
		dr2 = cr2 - ci5;
		di5 = ci2 - cr5;
		di2 = ci2 + cr5;
		ch[(m2 + (k + (i__ * 5 + 2) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + wa_dim1 * 5] * dr2 - wa[i__ + wa_dim1 * 9] * 
			di2;
		ch[(m2 + (k + (i__ * 5 + 2) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + wa_dim1 * 5] * di2 + wa[i__ + wa_dim1 * 9] * 
			dr2;
		ch[(m2 + (k + (i__ * 5 + 3) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + wa_dim1 * 6] * dr3 - wa[i__ + wa_dim1 * 10] *
			 di3;
		ch[(m2 + (k + (i__ * 5 + 3) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + wa_dim1 * 6] * di3 + wa[i__ + wa_dim1 * 10] *
			 dr3;
		ch[(m2 + (k + (i__ * 5 + 4) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + wa_dim1 * 7] * dr4 - wa[i__ + wa_dim1 * 11] *
			 di4;
		ch[(m2 + (k + (i__ * 5 + 4) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + wa_dim1 * 7] * di4 + wa[i__ + wa_dim1 * 11] *
			 dr4;
		ch[(m2 + (k + (i__ * 5 + 5) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + (wa_dim1 << 3)] * dr5 - wa[i__ + wa_dim1 * 
			12] * di5;
		ch[(m2 + (k + (i__ * 5 + 5) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + (wa_dim1 << 3)] * di5 + wa[i__ + wa_dim1 * 
			12] * dr5;
/* L104: */
	    }
	}
/* L105: */
    }
    return 0;
} /* cmf5kb_ */

