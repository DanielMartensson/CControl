/* c1f2kf.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int c1f2kf_(integer *ido, integer *l1, integer *na, real *cc,
	 integer *in1, real *ch, integer *in2, real *wa)
{
    /* System generated locals */
    integer cc_dim1, cc_dim2, cc_dim3, cc_offset, ch_dim1, ch_dim2, ch_offset,
	     wa_dim1, wa_offset, i__1, i__2;

    /* Local variables */
    static integer i__, k;
    static real sn, ti2, tr2, chold1, chold2;


    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_offset = 1 + (wa_dim1 << 1);
    wa -= wa_offset;
    cc_dim1 = *in1;
    cc_dim2 = *l1;
    cc_dim3 = *ido;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2 * (1 + cc_dim3));
    cc -= cc_offset;
    ch_dim1 = *in2;
    ch_dim2 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2 * 3);
    ch -= ch_offset;

    /* Function Body */
    if (*ido > 1) {
	goto L102;
    }
    sn = 1.f / (real) (*l1 << 1);
    if (*na == 1) {
	goto L106;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	chold1 = sn * (cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] + cc[(
		k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1]);
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 1] = sn * (cc[(k 
		+ (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] - cc[(k + ((cc_dim3 
		<< 1) + 1) * cc_dim2) * cc_dim1 + 1]);
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] = chold1;
	chold2 = sn * (cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] + cc[(
		k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2]);
	cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * cc_dim1 + 2] = sn * (cc[(k 
		+ (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] - cc[(k + ((cc_dim3 
		<< 1) + 1) * cc_dim2) * cc_dim1 + 2]);
	cc[(k + (cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] = chold2;
/* L101: */
    }
    return 0;
L106:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = sn * (cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 1] + cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 1]);
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = sn * (cc[(k + (cc_dim3 + 1) *
		 cc_dim2) * cc_dim1 + 1] - cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 1]);
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = sn * (cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 2] + cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 2]);
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = sn * (cc[(k + (cc_dim3 + 1) *
		 cc_dim2) * cc_dim1 + 2] - cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 2]);
/* L107: */
    }
    return 0;
L102:
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	ch[(k + ch_dim2 * 3) * ch_dim1 + 1] = cc[(k + (cc_dim3 + 1) * cc_dim2)
		 * cc_dim1 + 1] + cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * 
		cc_dim1 + 1];
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 1] = cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 1] - cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 1];
	ch[(k + ch_dim2 * 3) * ch_dim1 + 2] = cc[(k + (cc_dim3 + 1) * cc_dim2)
		 * cc_dim1 + 2] + cc[(k + ((cc_dim3 << 1) + 1) * cc_dim2) * 
		cc_dim1 + 2];
	ch[(k + (ch_dim2 << 2)) * ch_dim1 + 2] = cc[(k + (cc_dim3 + 1) * 
		cc_dim2) * cc_dim1 + 2] - cc[(k + ((cc_dim3 << 1) + 1) * 
		cc_dim2) * cc_dim1 + 2];
/* L103: */
    }
    i__1 = *ido;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[(k + ((i__ << 1) + 1) * ch_dim2) * ch_dim1 + 1] = cc[(k + (i__ 
		    + cc_dim3) * cc_dim2) * cc_dim1 + 1] + cc[(k + (i__ + (
		    cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 1];
	    tr2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 1] - cc[(k + 
		    (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 1];
	    ch[(k + ((i__ << 1) + 1) * ch_dim2) * ch_dim1 + 2] = cc[(k + (i__ 
		    + cc_dim3) * cc_dim2) * cc_dim1 + 2] + cc[(k + (i__ + (
		    cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 2];
	    ti2 = cc[(k + (i__ + cc_dim3) * cc_dim2) * cc_dim1 + 2] - cc[(k + 
		    (i__ + (cc_dim3 << 1)) * cc_dim2) * cc_dim1 + 2];
	    ch[(k + ((i__ << 1) + 2) * ch_dim2) * ch_dim1 + 2] = wa[i__ + (
		    wa_dim1 << 1)] * ti2 - wa[i__ + wa_dim1 * 3] * tr2;
	    ch[(k + ((i__ << 1) + 2) * ch_dim2) * ch_dim1 + 1] = wa[i__ + (
		    wa_dim1 << 1)] * tr2 + wa[i__ + wa_dim1 * 3] * ti2;
/* L104: */
	}
/* L105: */
    }
    return 0;
} /* c1f2kf_ */

