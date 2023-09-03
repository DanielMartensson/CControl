/* cmf3kf.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cmf3kf_(integer *lot, integer *ido, integer *l1, integer 
	*na, real *cc, integer *im1, integer *in1, real *ch, integer *im2, 
	integer *in2, real *wa)
{
    /* Initialized data */

    static real taur = -.5f;
    static real taui = -.866025403784439f;

    /* System generated locals */
    integer cc_dim2, cc_dim3, cc_dim4, cc_offset, ch_dim2, ch_dim3, ch_offset,
	     wa_dim1, wa_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, k, m1, m2;
    static real sn, ci2, ci3, di2, di3;
    static integer m1d;
    static real cr2, cr3, dr2, dr3, ti2;
    static integer m2s;
    static real tr2;

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_offset = 1 + wa_dim1 * 3;
    wa -= wa_offset;
    cc_dim2 = *in1;
    cc_dim3 = *l1;
    cc_dim4 = *ido;
    cc_offset = 1 + 2 * (1 + cc_dim2 * (1 + cc_dim3 * (1 + cc_dim4)));
    cc -= cc_offset;
    ch_dim2 = *in2;
    ch_dim3 = *l1;
    ch_offset = 1 + 2 * (1 + ch_dim2 * (1 + (ch_dim3 << 2)));
    ch -= ch_offset;

    /* Function Body */

    m1d = (*lot - 1) * *im1 + 1;
    m2s = 1 - *im2;
    if (*ido > 1) {
	goto L102;
    }
    sn = 1.f / (real) (*l1 * 3);
    if (*na == 1) {
	goto L106;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = m1d;
	i__3 = *im1;
	for (m1 = 1; i__3 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__3) {
	    tr2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    cr2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + taur * tr2;
	    cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] = sn *
		     (cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 1] + tr2);
	    ti2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ci2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + taur * ti2;
	    cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] = sn *
		     (cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) 
		    + 2] + ti2);
	    cr3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 1]);
	    ci3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 2]);
	    cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 1) + 1]
		     = sn * (cr2 - ci3);
	    cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) + 1] = 
		    sn * (cr2 + ci3);
	    cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 1) + 2]
		     = sn * (ci2 + cr3);
	    cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * cc_dim2 << 1) + 2] = 
		    sn * (ci2 - cr3);
/* L101: */
	}
    }
    return 0;
L106:
    i__3 = *l1;
    for (k = 1; k <= i__3; ++k) {
	m2 = m2s;
	i__2 = m1d;
	i__1 = *im1;
	for (m1 = 1; i__1 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__1) {
	    m2 += *im2;
	    tr2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    cr2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + taur * tr2;
	    ch[(m2 + (k + (ch_dim3 << 2)) * ch_dim2 << 1) + 1] = sn * (cc[(m1 
		    + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] + 
		    tr2);
	    ti2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ci2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + taur * ti2;
	    ch[(m2 + (k + (ch_dim3 << 2)) * ch_dim2 << 1) + 2] = sn * (cc[(m1 
		    + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] + 
		    ti2);
	    cr3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 1]);
	    ci3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 2]);
	    ch[(m2 + (k + ch_dim3 * 5) * ch_dim2 << 1) + 1] = sn * (cr2 - ci3)
		    ;
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 1] = sn * (cr2 + ci3)
		    ;
	    ch[(m2 + (k + ch_dim3 * 5) * ch_dim2 << 1) + 2] = sn * (ci2 + cr3)
		    ;
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 2] = sn * (ci2 - cr3)
		    ;
/* L107: */
	}
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	m2 = m2s;
	i__2 = m1d;
	i__3 = *im1;
	for (m1 = 1; i__3 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__3) {
	    m2 += *im2;
	    tr2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 1] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1];
	    cr2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] 
		    + taur * tr2;
	    ch[(m2 + (k + (ch_dim3 << 2)) * ch_dim2 << 1) + 1] = cc[(m1 + (k 
		    + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 1] + tr2;
	    ti2 = cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * cc_dim2 << 
		    1) + 2] + cc[(m1 + (k + (cc_dim4 * 3 + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2];
	    ci2 = cc[(m1 + (k + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] 
		    + taur * ti2;
	    ch[(m2 + (k + (ch_dim3 << 2)) * ch_dim2 << 1) + 2] = cc[(m1 + (k 
		    + (cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) + 2] + ti2;
	    cr3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 1] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 1]);
	    ci3 = taui * (cc[(m1 + (k + ((cc_dim4 << 1) + 1) * cc_dim3) * 
		    cc_dim2 << 1) + 2] - cc[(m1 + (k + (cc_dim4 * 3 + 1) * 
		    cc_dim3) * cc_dim2 << 1) + 2]);
	    ch[(m2 + (k + ch_dim3 * 5) * ch_dim2 << 1) + 1] = cr2 - ci3;
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 1] = cr2 + ci3;
	    ch[(m2 + (k + ch_dim3 * 5) * ch_dim2 << 1) + 2] = ci2 + cr3;
	    ch[(m2 + (k + ch_dim3 * 6) * ch_dim2 << 1) + 2] = ci2 - cr3;
/* L103: */
	}
    }
    i__3 = *ido;
    for (i__ = 2; i__ <= i__3; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    m2 = m2s;
	    i__1 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__1 : m1 <= i__1; m1 += i__4) {
		m2 += *im2;
		tr2 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 1] + cc[(m1 + (k + (i__ + cc_dim4 * 3)
			 * cc_dim3) * cc_dim2 << 1) + 1];
		cr2 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 1] + taur * tr2;
		ch[(m2 + (k + (i__ * 3 + 1) * ch_dim3) * ch_dim2 << 1) + 1] = 
			cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 
			1) + 1] + tr2;
		ti2 = cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) * 
			cc_dim2 << 1) + 2] + cc[(m1 + (k + (i__ + cc_dim4 * 3)
			 * cc_dim3) * cc_dim2 << 1) + 2];
		ci2 = cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 1)
			 + 2] + taur * ti2;
		ch[(m2 + (k + (i__ * 3 + 1) * ch_dim3) * ch_dim2 << 1) + 2] = 
			cc[(m1 + (k + (i__ + cc_dim4) * cc_dim3) * cc_dim2 << 
			1) + 2] + ti2;
		cr3 = taui * (cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) 
			* cc_dim2 << 1) + 1] - cc[(m1 + (k + (i__ + cc_dim4 * 
			3) * cc_dim3) * cc_dim2 << 1) + 1]);
		ci3 = taui * (cc[(m1 + (k + (i__ + (cc_dim4 << 1)) * cc_dim3) 
			* cc_dim2 << 1) + 2] - cc[(m1 + (k + (i__ + cc_dim4 * 
			3) * cc_dim3) * cc_dim2 << 1) + 2]);
		dr2 = cr2 - ci3;
		dr3 = cr2 + ci3;
		di2 = ci2 + cr3;
		di3 = ci2 - cr3;
		ch[(m2 + (k + (i__ * 3 + 2) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + wa_dim1 * 3] * di2 - wa[i__ + wa_dim1 * 5] * 
			dr2;
		ch[(m2 + (k + (i__ * 3 + 2) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + wa_dim1 * 3] * dr2 + wa[i__ + wa_dim1 * 5] * 
			di2;
		ch[(m2 + (k + (i__ * 3 + 3) * ch_dim3) * ch_dim2 << 1) + 2] = 
			wa[i__ + (wa_dim1 << 2)] * di3 - wa[i__ + wa_dim1 * 6]
			 * dr3;
		ch[(m2 + (k + (i__ * 3 + 3) * ch_dim3) * ch_dim2 << 1) + 1] = 
			wa[i__ + (wa_dim1 << 2)] * dr3 + wa[i__ + wa_dim1 * 6]
			 * di3;
/* L104: */
	    }
	}
/* L105: */
    }
    return 0;
} /* cmf3kf_ */

