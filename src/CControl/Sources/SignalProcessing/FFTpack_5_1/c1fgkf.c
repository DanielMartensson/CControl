/* c1fgkf.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int c1fgkf_(integer *ido, integer *ip, integer *l1, integer *
	lid, integer *na, real *cc, real *cc1, integer *in1, real *ch, real *
	ch1, integer *in2, real *wa)
{
    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_dim3, ch_offset, cc_dim1, cc_dim2, cc_dim3, 
	    cc_offset, cc1_dim1, cc1_dim2, cc1_offset, ch1_dim1, ch1_dim2, 
	    ch1_offset, wa_dim1, wa_dim2, wa_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l, jc, lc, ki;
    static real sn, wai, war;
    static integer ipp2, idlj, ipph;
    static real chold1, chold2;


/* FFTPACK 5.1 auxiliary routine */

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_dim2 = *ip - 1;
    wa_offset = 1 + wa_dim1 * (1 + wa_dim2);
    wa -= wa_offset;
    cc1_dim1 = *in1;
    cc1_dim2 = *lid;
    cc1_offset = 1 + cc1_dim1 * (1 + cc1_dim2);
    cc1 -= cc1_offset;
    cc_dim1 = *in1;
    cc_dim2 = *l1;
    cc_dim3 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2 * (1 + cc_dim3));
    cc -= cc_offset;
    ch1_dim1 = *in2;
    ch1_dim2 = *lid;
    ch1_offset = 1 + ch1_dim1 * (1 + ch1_dim2);
    ch1 -= ch1_offset;
    ch_dim1 = *in2;
    ch_dim2 = *l1;
    ch_dim3 = *ido;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2 * (1 + ch_dim3));
    ch -= ch_offset;

    /* Function Body */
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    i__1 = *lid;
    for (ki = 1; ki <= i__1; ++ki) {
	ch1[(ki + ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + cc1_dim2) * cc1_dim1 
		+ 1];
	ch1[(ki + ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + cc1_dim2) * cc1_dim1 
		+ 2];
/* L110: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + j * cc1_dim2) 
		    * cc1_dim1 + 1] + cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 1]
		    ;
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + j * cc1_dim2)
		     * cc1_dim1 + 1] - cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 
		    1];
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + j * cc1_dim2) 
		    * cc1_dim1 + 2] + cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 2]
		    ;
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + j * cc1_dim2)
		     * cc1_dim1 + 2] - cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 
		    2];
/* L112: */
	}
/* L111: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    cc1[(ki + cc1_dim2) * cc1_dim1 + 1] += ch1[(ki + j * ch1_dim2) * 
		    ch1_dim1 + 1];
	    cc1[(ki + cc1_dim2) * cc1_dim1 + 2] += ch1[(ki + j * ch1_dim2) * 
		    ch1_dim1 + 2];
/* L117: */
	}
/* L118: */
    }
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    cc1[(ki + l * cc1_dim2) * cc1_dim1 + 1] = ch1[(ki + ch1_dim2) * 
		    ch1_dim1 + 1] + wa[(l - 1 + wa_dim2) * wa_dim1 + 1] * ch1[
		    (ki + (ch1_dim2 << 1)) * ch1_dim1 + 1];
	    cc1[(ki + lc * cc1_dim2) * cc1_dim1 + 1] = -wa[(l - 1 + (wa_dim2 
		    << 1)) * wa_dim1 + 1] * ch1[(ki + *ip * ch1_dim2) * 
		    ch1_dim1 + 1];
	    cc1[(ki + l * cc1_dim2) * cc1_dim1 + 2] = ch1[(ki + ch1_dim2) * 
		    ch1_dim1 + 2] + wa[(l - 1 + wa_dim2) * wa_dim1 + 1] * ch1[
		    (ki + (ch1_dim2 << 1)) * ch1_dim1 + 2];
	    cc1[(ki + lc * cc1_dim2) * cc1_dim1 + 2] = -wa[(l - 1 + (wa_dim2 
		    << 1)) * wa_dim1 + 1] * ch1[(ki + *ip * ch1_dim2) * 
		    ch1_dim1 + 2];
/* L113: */
	}
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    idlj = (l - 1) * (j - 1) % *ip;
	    war = wa[(idlj + wa_dim2) * wa_dim1 + 1];
	    wai = -wa[(idlj + (wa_dim2 << 1)) * wa_dim1 + 1];
	    i__3 = *lid;
	    for (ki = 1; ki <= i__3; ++ki) {
		cc1[(ki + l * cc1_dim2) * cc1_dim1 + 1] += war * ch1[(ki + j *
			 ch1_dim2) * ch1_dim1 + 1];
		cc1[(ki + lc * cc1_dim2) * cc1_dim1 + 1] += wai * ch1[(ki + 
			jc * ch1_dim2) * ch1_dim1 + 1];
		cc1[(ki + l * cc1_dim2) * cc1_dim1 + 2] += war * ch1[(ki + j *
			 ch1_dim2) * ch1_dim1 + 2];
		cc1[(ki + lc * cc1_dim2) * cc1_dim1 + 2] += wai * ch1[(ki + 
			jc * ch1_dim2) * ch1_dim1 + 2];
/* L114: */
	    }
/* L115: */
	}
/* L116: */
    }
    if (*ido > 1) {
	goto L136;
    }
    sn = 1.f / (real) (*ip * *l1);
    if (*na == 1) {
	goto L146;
    }
    i__1 = *lid;
    for (ki = 1; ki <= i__1; ++ki) {
	cc1[(ki + cc1_dim2) * cc1_dim1 + 1] = sn * cc1[(ki + cc1_dim2) * 
		cc1_dim1 + 1];
	cc1[(ki + cc1_dim2) * cc1_dim1 + 2] = sn * cc1[(ki + cc1_dim2) * 
		cc1_dim1 + 2];
/* L149: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    chold1 = sn * (cc1[(ki + j * cc1_dim2) * cc1_dim1 + 1] - cc1[(ki 
		    + jc * cc1_dim2) * cc1_dim1 + 2]);
	    chold2 = sn * (cc1[(ki + j * cc1_dim2) * cc1_dim1 + 1] + cc1[(ki 
		    + jc * cc1_dim2) * cc1_dim1 + 2]);
	    cc1[(ki + j * cc1_dim2) * cc1_dim1 + 1] = chold1;
	    cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 2] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 2] - cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 1]);
	    cc1[(ki + j * cc1_dim2) * cc1_dim1 + 2] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 2] + cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 1]);
	    cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 1] = chold2;
/* L119: */
	}
/* L120: */
    }
    return 0;
L146:
    i__1 = *lid;
    for (ki = 1; ki <= i__1; ++ki) {
	ch1[(ki + ch1_dim2) * ch1_dim1 + 1] = sn * cc1[(ki + cc1_dim2) * 
		cc1_dim1 + 1];
	ch1[(ki + ch1_dim2) * ch1_dim1 + 2] = sn * cc1[(ki + cc1_dim2) * 
		cc1_dim1 + 2];
/* L147: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 1] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 1] - cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 2]);
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 2] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 2] + cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 1]);
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 1] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 1] + cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 2]);
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 2] = sn * (cc1[(ki + j * 
		    cc1_dim2) * cc1_dim1 + 2] - cc1[(ki + jc * cc1_dim2) * 
		    cc1_dim1 + 1]);
/* L144: */
	}
/* L145: */
    }
    return 0;
L136:
    i__1 = *lid;
    for (ki = 1; ki <= i__1; ++ki) {
	ch1[(ki + ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + cc1_dim2) * cc1_dim1 
		+ 1];
	ch1[(ki + ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + cc1_dim2) * cc1_dim1 
		+ 2];
/* L137: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + j * cc1_dim2) 
		    * cc1_dim1 + 1] - cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 2]
		    ;
	    ch1[(ki + j * ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + j * cc1_dim2) 
		    * cc1_dim1 + 2] + cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 1]
		    ;
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 1] = cc1[(ki + j * cc1_dim2)
		     * cc1_dim1 + 1] + cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 
		    2];
	    ch1[(ki + jc * ch1_dim2) * ch1_dim1 + 2] = cc1[(ki + j * cc1_dim2)
		     * cc1_dim1 + 2] - cc1[(ki + jc * cc1_dim2) * cc1_dim1 + 
		    1];
/* L134: */
	}
/* L135: */
    }
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    cc[(k + (i__ * cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1] = ch[(k + (
		    i__ + ch_dim3) * ch_dim2) * ch_dim1 + 1];
	    cc[(k + (i__ * cc_dim3 + 1) * cc_dim2) * cc_dim1 + 2] = ch[(k + (
		    i__ + ch_dim3) * ch_dim2) * ch_dim1 + 2];
/* L130: */
	}
/* L131: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    cc[(k + (j + cc_dim3) * cc_dim2) * cc_dim1 + 1] = ch[(k + (j * 
		    ch_dim3 + 1) * ch_dim2) * ch_dim1 + 1];
	    cc[(k + (j + cc_dim3) * cc_dim2) * cc_dim1 + 2] = ch[(k + (j * 
		    ch_dim3 + 1) * ch_dim2) * ch_dim1 + 2];
/* L122: */
	}
/* L123: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *ido;
	for (i__ = 2; i__ <= i__2; ++i__) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		cc[(k + (j + i__ * cc_dim3) * cc_dim2) * cc_dim1 + 1] = wa[
			i__ + (j - 1 + wa_dim2) * wa_dim1] * ch[(k + (i__ + j 
			* ch_dim3) * ch_dim2) * ch_dim1 + 1] + wa[i__ + (j - 
			1 + (wa_dim2 << 1)) * wa_dim1] * ch[(k + (i__ + j * 
			ch_dim3) * ch_dim2) * ch_dim1 + 2];
		cc[(k + (j + i__ * cc_dim3) * cc_dim2) * cc_dim1 + 2] = wa[
			i__ + (j - 1 + wa_dim2) * wa_dim1] * ch[(k + (i__ + j 
			* ch_dim3) * ch_dim2) * ch_dim1 + 2] - wa[i__ + (j - 
			1 + (wa_dim2 << 1)) * wa_dim1] * ch[(k + (i__ + j * 
			ch_dim3) * ch_dim2) * ch_dim1 + 1];
/* L124: */
	    }
/* L125: */
	}
/* L126: */
    }
    return 0;
} /* c1fgkf_ */

