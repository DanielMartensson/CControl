/* c1f3kf.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int c1f3kf_(integer *ido, integer *l1, integer *na, real *cc,
	 integer *in1, real *ch, integer *in2, real *wa)
{
    /* Initialized data */

    static real taur = -.5f;
    static real taui = -.866025403784439f;

    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_dim3, cc_offset, ch_dim1, ch_dim2, ch_offset,
	     wa_dim1, wa_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static real sn, ci2, ci3, di2, di3, cr2, cr3, dr2, dr3, ti2, tr2;

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_offset = 1 + wa_dim1 * 3;
    wa -= wa_offset;
    cc_dim1 = *in1;
    cc_dim2 = *l1;
    cc_dim3 = *ido;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2 * (1 + cc_dim3));
    cc -= cc_offset;
    ch_dim1 = *in2;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + (ch_dim2 << 2));
    ch -= ch_offset;

    /* Function Body */

    if (*ido > 1) {
	goto L102;
    }
    sn = 1.f / (real) (*l1 * 3);
    if (*na == 1) {
	goto L106;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1];
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + taur * tr2;
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cc[(k + (
		cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + tr2);
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2];
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + taur * ti2;
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] = sn * (cc[(k + (
		cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + ti2);
	cr3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1]);
	ci3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2]);
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr2 - 
		ci3);
	cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cr2 + ci3)
		;
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci2 + 
		cr3);
	cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2] = sn * (ci2 - cr3)
		;
/* L101: */
    }
    return 0;
L106:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1];
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + taur * tr2;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = sn * (cc[(k + (cc_dim3 + 1) *
		 cc_dim2) * cc_dim1 + 1] + tr2);
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2];
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + taur * ti2;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = sn * (cc[(k + (cc_dim3 + 1) *
		 cc_dim2) * cc_dim1 + 2] + ti2);
	cr3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1]);
	ci3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2]);
	ch[(k + ch_dim2 * 5) * ch_dim1 + 1] = sn * (cr2 - ci3);
	ch[(k + ch_dim2 * 6) * ch_dim1 + 1] = sn * (cr2 + ci3);
	ch[(k + ch_dim2 * 5) * ch_dim1 + 2] = sn * (ci2 + cr3);
	ch[(k + ch_dim2 * 6) * ch_dim1 + 2] = sn * (ci2 - cr3);
/* L107: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	tr2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1];
	cr2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + taur * tr2;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 1] + tr2;
	ti2 = cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] + cc[(k 
		+ (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2];
	ci2 = cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + taur * ti2;
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 2] + ti2;
	cr3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 1]);
	ci3 = taui * (cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] 
		- cc[(k + (cc_dim3 * 3 + 1) * cc_dim2) * cc_dim1 + 2]);
	ch[(k + ch_dim2 * 5) * ch_dim1 + 1] = cr2 - ci3;
	ch[(k + ch_dim2 * 6) * ch_dim1 + 1] = cr2 + ci3;
	ch[(k + ch_dim2 * 5) * ch_dim1 + 2] = ci2 + cr3;
	ch[(k + ch_dim2 * 6) * ch_dim1 + 2] = ci2 - cr3;
/* L103: */
    }
    i__1 = *ido;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    tr2 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 1] + 
		    cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 1];
	    cr2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 1] + taur * 
		    tr2;
	    ch[(k + (i__ * 3 + 1) * ch_dim2) * ch_dim1 + 1] = cc[(k + (i__ + 
		    cc_dim3) * cc_dim2) * cc_dim1 + 1] + tr2;
	    ti2 = cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 2] + 
		    cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 2];
	    ci2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 2] + taur * 
		    ti2;
	    ch[(k + (i__ * 3 + 1) * ch_dim2) * ch_dim1 + 2] = cc[(k + (i__ + 
		    cc_dim3) * cc_dim2) * cc_dim1 + 2] + ti2;
	    cr3 = taui * (cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 
		    + 1] - cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 
		    1]);
	    ci3 = taui * (cc[(k + (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 
		    + 2] - cc[(k + (i__ + cc_dim3 * 3) * cc_dim2) * cc_dim1 + 
		    2]);
	    dr2 = cr2 - ci3;
	    dr3 = cr2 + ci3;
	    di2 = ci2 + cr3;
	    di3 = ci2 - cr3;
	    ch[(k + (i__ * 3 + 2) * ch_dim2) * ch_dim1 + 2] = wa[i__ + 
		    wa_dim1 * 3] * di2 - wa[i__ + wa_dim1 * 5] * dr2;
	    ch[(k + (i__ * 3 + 2) * ch_dim2) * ch_dim1 + 1] = wa[i__ + 
		    wa_dim1 * 3] * dr2 + wa[i__ + wa_dim1 * 5] * di2;
	    ch[(k + (i__ * 3 + 3) * ch_dim2) * ch_dim1 + 2] = wa[i__ + (
		    wa_dim1 << 2)] * di3 - wa[i__ + wa_dim1 * 6] * dr3;
	    ch[(k + (i__ * 3 + 3) * ch_dim2) * ch_dim1 + 1] = wa[i__ + (
		    wa_dim1 << 2)] * dr3 + wa[i__ + wa_dim1 * 6] * di3;
/* L104: */
	}
/* L105: */
    }
    return 0;
} /* c1f3kf_ */

