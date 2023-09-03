/* c1f5kf.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int c1f5kf_(integer *ido, integer *l1, integer *na, real *cc,
	 integer *in1, real *ch, integer *in2, real *wa)
{
    /* Initialized data */

    static real tr11 = .3090169943749474f;
    static real ti11 = -.9510565162951536f;
    static real tr12 = -.8090169943749474f;
    static real ti12 = -.5877852522924731f;

    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_dim3, cc_offset, ch_dim1, ch_dim2, ch_offset,
	     wa_dim1, wa_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static real sn, ci2, ci3, ci4, ci5, di3, di4, di5, di2, cr2, cr3, cr5, 
	    cr4, dr3, dr4, ti2, ti3, ti4, ti5, dr5, dr2, tr2, tr3, tr4, tr5, 
	    chold1, chold2;

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_offset = 1 + wa_dim1 * 5;
    wa -= wa_offset;
    cc_dim1 = *in1;
    cc_dim2 = *l1;
    cc_dim3 = *ido;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2 * (1 + cc_dim3));
    cc -= cc_offset;
    ch_dim1 = *in2;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2 * 6);
    ch -= ch_offset;

    /* Function Body */

/* FFTPACK 5.1 auxiliary routine */

    if (*ido > 1) {
	goto L102;
    }
    sn = 1.f / (real) (*l1 * 5);
    if (*na == 1) {
	goto L106;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	ti3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	tr5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	tr3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	chold1 = sn * (cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr2 
		+ tr3);
	chold2 = sn * (cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + ti2 
		+ ti3);
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr11 * tr2 + 
		tr12 * tr3;
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr11 * ti2 + 
		tr12 * ti3;
	cr3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr12 * tr2 + 
		tr11 * tr3;
	ci3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr12 * ti2 + 
		tr11 * ti3;
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] = chold1;
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] = chold2;
	cr5 = ti11 * tr5 + ti12 * tr4;
	ci5 = ti11 * ti5 + ti12 * ti4;
	cr4 = ti12 * tr5 - ti11 * tr4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr2 - 
		ci5);
	cc[(k + (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr2 + ci5)
		;
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci2 + 
		cr5);
	cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci3 + cr4)
		;
	cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr3 - ci4)
		;
	cc[(k + ((cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr3 + 
		ci4);
	cc[(k + ((cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci3 - 
		cr4);
	cc[(k + (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci2 - cr5)
		;
/* L101: */
    }
    return 0;
L106:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	ti3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	tr5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	tr3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	ch[(k + ch_dim2 * 6) * ch_dim1 + 1] = sn * (cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 1] + tr2 + tr3);
	ch[(k + ch_dim2 * 6) * ch_dim1 + 2] = sn * (cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 2] + ti2 + ti3);
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr11 * tr2 + 
		tr12 * tr3;
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr11 * ti2 + 
		tr12 * ti3;
	cr3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr12 * tr2 + 
		tr11 * tr3;
	ci3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr12 * ti2 + 
		tr11 * ti3;
	cr5 = ti11 * tr5 + ti12 * tr4;
	ci5 = ti11 * ti5 + ti12 * ti4;
	cr4 = ti12 * tr5 - ti11 * tr4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	ch[(k + ch_dim2 * 7) * ch_dim1 + 1] = sn * (cr2 - ci5);
	ch[(k + ch_dim2 * 10) * ch_dim1 + 1] = sn * (cr2 + ci5);
	ch[(k + ch_dim2 * 7) * ch_dim1 + 2] = sn * (ci2 + cr5);
	ch[(k + (ch_dim2 << 3)) * ch_dim1 + 2] = sn * (ci3 + cr4);
	ch[(k + (ch_dim2 << 3)) * ch_dim1 + 1] = sn * (cr3 - ci4);
	ch[(k + ch_dim2 * 9) * ch_dim1 + 1] = sn * (cr3 + ci4);
	ch[(k + ch_dim2 * 9) * ch_dim1 + 2] = sn * (ci3 - cr4);
	ch[(k + ch_dim2 * 10) * ch_dim1 + 2] = sn * (ci2 - cr5);
/* L107: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ti5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 2];
	ti4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	ti3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 2];
	tr5 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 5 + 1) * cc_dim2) * cc_dim1 + 1];
	tr4 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	tr3 = cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k + ((
		cc_dim3 << 2) + 1) * cc_dim2) * cc_dim1 + 1];
	ch[(k + ch_dim2 * 6) * ch_dim1 + 1] = cc[(k + (cc_dim3 + 1) * cc_dim2)
		 * cc_dim1 + 1] + tr2 + tr3;
	ch[(k + ch_dim2 * 6) * ch_dim1 + 2] = cc[(k + (cc_dim3 + 1) * cc_dim2)
		 * cc_dim1 + 2] + ti2 + ti3;
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr11 * tr2 + 
		tr12 * tr3;
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr11 * ti2 + 
		tr12 * ti3;
	cr3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr12 * tr2 + 
		tr11 * tr3;
	ci3 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + tr12 * ti2 + 
		tr11 * ti3;
	cr5 = ti11 * tr5 + ti12 * tr4;
	ci5 = ti11 * ti5 + ti12 * ti4;
	cr4 = ti12 * tr5 - ti11 * tr4;
	ci4 = ti12 * ti5 - ti11 * ti4;
	ch[(k + ch_dim2 * 7) * ch_dim1 + 1] = cr2 - ci5;
	ch[(k + ch_dim2 * 10) * ch_dim1 + 1] = cr2 + ci5;
	ch[(k + ch_dim2 * 7) * ch_dim1 + 2] = ci2 + cr5;
	ch[(k + (ch_dim2 << 3)) * ch_dim1 + 2] = ci3 + cr4;
	ch[(k + (ch_dim2 << 3)) * ch_dim1 + 1] = cr3 - ci4;
	ch[(k + ch_dim2 * 9) * ch_dim1 + 1] = cr3 + ci4;
	ch[(k + ch_dim2 * 9) * ch_dim1 + 2] = ci3 - cr4;
	ch[(k + ch_dim2 * 10) * ch_dim1 + 2] = ci2 - cr5;
/* L103: */
    }
    i__1 = *ido;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ti5 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 2] - 
		    cc[(k + (i__ + cc_dim3 * 5) * cc_dim2) * cc_dim1 + 2];
	    ti2 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 2] + 
		    cc[(k + (i__ + cc_dim3 * 5) * cc_dim2) * cc_dim1 + 2];
	    ti4 = cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 2] - cc[(
		    k + (i__ + (cc_dim3 << 2)) * cc_dim2) * cc_dim1 + 2];
	    ti3 = cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 2] + cc[(
		    k + (i__ + (cc_dim3 << 2)) * cc_dim2) * cc_dim1 + 2];
	    tr5 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 1] - 
		    cc[(k + (i__ + cc_dim3 * 5) * cc_dim2) * cc_dim1 + 1];
	    tr2 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 1] + 
		    cc[(k + (i__ + cc_dim3 * 5) * cc_dim2) * cc_dim1 + 1];
	    tr4 = cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 1] - cc[(
		    k + (i__ + (cc_dim3 << 2)) * cc_dim2) * cc_dim1 + 1];
	    tr3 = cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 1] + cc[(
		    k + (i__ + (cc_dim3 << 2)) * cc_dim2) * cc_dim1 + 1];
	    ch[(k + (i__ * 5 + 1) * ch_dim2) * ch_dim1 + 1] = cc[(k + (i__ + 
		    cc_dim3) * cc_dim2) * cc_dim1 + 1] + tr2 + tr3;
	    ch[(k + (i__ * 5 + 1) * ch_dim2) * ch_dim1 + 2] = cc[(k + (i__ + 
		    cc_dim3) * cc_dim2) * cc_dim1 + 2] + ti2 + ti3;
	    cr2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 1] + tr11 * 
		    tr2 + tr12 * tr3;
	    ci2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 2] + tr11 * 
		    ti2 + tr12 * ti3;
	    cr3 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 1] + tr12 * 
		    tr2 + tr11 * tr3;
	    ci3 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 2] + tr12 * 
		    ti2 + tr11 * ti3;
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
	    ch[(k + (i__ * 5 + 2) * ch_dim2) * ch_dim1 + 1] = wa[i__ + 
		    wa_dim1 * 5] * dr2 + wa[i__ + wa_dim1 * 9] * di2;
	    ch[(k + (i__ * 5 + 2) * ch_dim2) * ch_dim1 + 2] = wa[i__ + 
		    wa_dim1 * 5] * di2 - wa[i__ + wa_dim1 * 9] * dr2;
	    ch[(k + (i__ * 5 + 3) * ch_dim2) * ch_dim1 + 1] = wa[i__ + 
		    wa_dim1 * 6] * dr3 + wa[i__ + wa_dim1 * 10] * di3;
	    ch[(k + (i__ * 5 + 3) * ch_dim2) * ch_dim1 + 2] = wa[i__ + 
		    wa_dim1 * 6] * di3 - wa[i__ + wa_dim1 * 10] * dr3;
	    ch[(k + (i__ * 5 + 4) * ch_dim2) * ch_dim1 + 1] = wa[i__ + 
		    wa_dim1 * 7] * dr4 + wa[i__ + wa_dim1 * 11] * di4;
	    ch[(k + (i__ * 5 + 4) * ch_dim2) * ch_dim1 + 2] = wa[i__ + 
		    wa_dim1 * 7] * di4 - wa[i__ + wa_dim1 * 11] * dr4;
	    ch[(k + (i__ * 5 + 5) * ch_dim2) * ch_dim1 + 1] = wa[i__ + (
		    wa_dim1 << 3)] * dr5 + wa[i__ + wa_dim1 * 12] * di5;
	    ch[(k + (i__ * 5 + 5) * ch_dim2) * ch_dim1 + 2] = wa[i__ + (
		    wa_dim1 << 3)] * di5 - wa[i__ + wa_dim1 * 12] * dr5;
/* L104: */
	}
/* L105: */
    }
    return 0;
} /* c1f5kf_ */

