/* r1fgkb.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int r1fgkb_(integer *ido, integer *ip, integer *l1, integer *
	idl1, real *cc, real *c1, real *c2, integer *in1, real *ch, real *ch2,
	 integer *in2, real *wa)
{
    /* System generated locals */
    integer ch_dim1, ch_dim2, ch_dim3, ch_offset, cc_dim1, cc_dim2, cc_dim3, 
	    cc_offset, c1_dim1, c1_dim2, c1_dim3, c1_offset, c2_dim1, c2_dim2,
	     c2_offset, ch2_dim1, ch2_dim2, ch2_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j, k, l, j2, ic, jc, lc, ik, is;
    static real dc2, ai1, ai2, ar1, ar2, ds2;
    static integer nbd;
    static real dcp, arg, dsp, tpi, ar1h, ar2h;
    static integer idp2, ipp2, idij, ipph;


    /* Parameter adjustments */
    --wa;
    c2_dim1 = *in1;
    c2_dim2 = *idl1;
    c2_offset = 1 + c2_dim1 * (1 + c2_dim2);
    c2 -= c2_offset;
    c1_dim1 = *in1;
    c1_dim2 = *ido;
    c1_dim3 = *l1;
    c1_offset = 1 + c1_dim1 * (1 + c1_dim2 * (1 + c1_dim3));
    c1 -= c1_offset;
    cc_dim1 = *in1;
    cc_dim2 = *ido;
    cc_dim3 = *ip;
    cc_offset = 1 + cc_dim1 * (1 + cc_dim2 * (1 + cc_dim3));
    cc -= cc_offset;
    ch2_dim1 = *in2;
    ch2_dim2 = *idl1;
    ch2_offset = 1 + ch2_dim1 * (1 + ch2_dim2);
    ch2 -= ch2_offset;
    ch_dim1 = *in2;
    ch_dim2 = *ido;
    ch_dim3 = *l1;
    ch_offset = 1 + ch_dim1 * (1 + ch_dim2 * (1 + ch_dim3));
    ch -= ch_offset;

    /* Function Body */
    tpi = atan(1.f) * 8.f;
    arg = tpi / (real) (*ip);
    dcp = cos(arg);
    dsp = sin(arg);
    idp2 = *ido + 2;
    nbd = (*ido - 1) / 2;
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    if (*ido < *l1) {
	goto L103;
    }
    i__1 = *l1;
    for (k = 1; k <= i__1; ++k) {
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ch[(i__ + (k + ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(i__ + (k *
		     cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1];
/* L101: */
	}
/* L102: */
    }
    goto L106;
L103:
    i__1 = *ido;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[(i__ + (k + ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(i__ + (k *
		     cc_dim3 + 1) * cc_dim2) * cc_dim1 + 1];
/* L104: */
	}
/* L105: */
    }
L106:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	j2 = j + j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[((k + j * ch_dim3) * ch_dim2 + 1) * ch_dim1 + 1] = cc[(*ido + (
		    j2 - 2 + k * cc_dim3) * cc_dim2) * cc_dim1 + 1] + cc[(*
		    ido + (j2 - 2 + k * cc_dim3) * cc_dim2) * cc_dim1 + 1];
	    ch[((k + jc * ch_dim3) * ch_dim2 + 1) * ch_dim1 + 1] = cc[((j2 - 
		    1 + k * cc_dim3) * cc_dim2 + 1) * cc_dim1 + 1] + cc[((j2 
		    - 1 + k * cc_dim3) * cc_dim2 + 1) * cc_dim1 + 1];
/* L1007: */
/* L107: */
	}
/* L108: */
    }
    if (*ido == 1) {
	goto L116;
    }
    if (nbd < *l1) {
	goto L112;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		ic = idp2 - i__;
		ch[(i__ - 1 + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			cc[(i__ - 1 + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) 
			* cc_dim1 + 1] + cc[(ic - 1 + ((j << 1) - 2 + k * 
			cc_dim3) * cc_dim2) * cc_dim1 + 1];
		ch[(i__ - 1 + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			cc[(i__ - 1 + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) 
			* cc_dim1 + 1] - cc[(ic - 1 + ((j << 1) - 2 + k * 
			cc_dim3) * cc_dim2) * cc_dim1 + 1];
		ch[(i__ + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(
			i__ + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) * 
			cc_dim1 + 1] - cc[(ic + ((j << 1) - 2 + k * cc_dim3) *
			 cc_dim2) * cc_dim1 + 1];
		ch[(i__ + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(
			i__ + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) * 
			cc_dim1 + 1] + cc[(ic + ((j << 1) - 2 + k * cc_dim3) *
			 cc_dim2) * cc_dim1 + 1];
/* L109: */
	    }
/* L110: */
	}
/* L111: */
    }
    goto L116;
L112:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    ic = idp2 - i__;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[(i__ - 1 + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			cc[(i__ - 1 + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) 
			* cc_dim1 + 1] + cc[(ic - 1 + ((j << 1) - 2 + k * 
			cc_dim3) * cc_dim2) * cc_dim1 + 1];
		ch[(i__ - 1 + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			cc[(i__ - 1 + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) 
			* cc_dim1 + 1] - cc[(ic - 1 + ((j << 1) - 2 + k * 
			cc_dim3) * cc_dim2) * cc_dim1 + 1];
		ch[(i__ + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(
			i__ + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) * 
			cc_dim1 + 1] - cc[(ic + ((j << 1) - 2 + k * cc_dim3) *
			 cc_dim2) * cc_dim1 + 1];
		ch[(i__ + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = cc[(
			i__ + ((j << 1) - 1 + k * cc_dim3) * cc_dim2) * 
			cc_dim1 + 1] + cc[(ic + ((j << 1) - 2 + k * cc_dim3) *
			 cc_dim2) * cc_dim1 + 1];
/* L113: */
	    }
/* L114: */
	}
/* L115: */
    }
L116:
    ar1 = 1.f;
    ai1 = 0.f;
    i__1 = ipph;
    for (l = 2; l <= i__1; ++l) {
	lc = ipp2 - l;
	ar1h = dcp * ar1 - dsp * ai1;
	ai1 = dcp * ai1 + dsp * ar1;
	ar1 = ar1h;
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    c2[(ik + l * c2_dim2) * c2_dim1 + 1] = ch2[(ik + ch2_dim2) * 
		    ch2_dim1 + 1] + ar1 * ch2[(ik + (ch2_dim2 << 1)) * 
		    ch2_dim1 + 1];
	    c2[(ik + lc * c2_dim2) * c2_dim1 + 1] = ai1 * ch2[(ik + *ip * 
		    ch2_dim2) * ch2_dim1 + 1];
/* L117: */
	}
	dc2 = ar1;
	ds2 = ai1;
	ar2 = ar1;
	ai2 = ai1;
	i__2 = ipph;
	for (j = 3; j <= i__2; ++j) {
	    jc = ipp2 - j;
	    ar2h = dc2 * ar2 - ds2 * ai2;
	    ai2 = dc2 * ai2 + ds2 * ar2;
	    ar2 = ar2h;
	    i__3 = *idl1;
	    for (ik = 1; ik <= i__3; ++ik) {
		c2[(ik + l * c2_dim2) * c2_dim1 + 1] += ar2 * ch2[(ik + j * 
			ch2_dim2) * ch2_dim1 + 1];
		c2[(ik + lc * c2_dim2) * c2_dim1 + 1] += ai2 * ch2[(ik + jc * 
			ch2_dim2) * ch2_dim1 + 1];
/* L118: */
	    }
/* L119: */
	}
/* L120: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *idl1;
	for (ik = 1; ik <= i__2; ++ik) {
	    ch2[(ik + ch2_dim2) * ch2_dim1 + 1] += ch2[(ik + j * ch2_dim2) * 
		    ch2_dim1 + 1];
/* L121: */
	}
/* L122: */
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    ch[((k + j * ch_dim3) * ch_dim2 + 1) * ch_dim1 + 1] = c1[((k + j *
		     c1_dim3) * c1_dim2 + 1) * c1_dim1 + 1] - c1[((k + jc * 
		    c1_dim3) * c1_dim2 + 1) * c1_dim1 + 1];
	    ch[((k + jc * ch_dim3) * ch_dim2 + 1) * ch_dim1 + 1] = c1[((k + j 
		    * c1_dim3) * c1_dim2 + 1) * c1_dim1 + 1] + c1[((k + jc * 
		    c1_dim3) * c1_dim2 + 1) * c1_dim1 + 1];
/* L123: */
	}
/* L124: */
    }
    if (*ido == 1) {
	goto L132;
    }
    if (nbd < *l1) {
	goto L128;
    }
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		ch[(i__ - 1 + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1] - c1[(i__ + (k + jc * c1_dim3) * c1_dim2) * 
			c1_dim1 + 1];
		ch[(i__ - 1 + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1] + c1[(i__ + (k + jc * c1_dim3) * c1_dim2) * 
			c1_dim1 + 1];
		ch[(i__ + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = c1[(
			i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] + 
			c1[(i__ - 1 + (k + jc * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1];
		ch[(i__ + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = c1[(
			i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] - 
			c1[(i__ - 1 + (k + jc * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1];
/* L125: */
	    }
/* L126: */
	}
/* L127: */
    }
    goto L132;
L128:
    i__1 = ipph;
    for (j = 2; j <= i__1; ++j) {
	jc = ipp2 - j;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		ch[(i__ - 1 + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1] - c1[(i__ + (k + jc * c1_dim3) * c1_dim2) * 
			c1_dim1 + 1];
		ch[(i__ - 1 + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = 
			c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1] + c1[(i__ + (k + jc * c1_dim3) * c1_dim2) * 
			c1_dim1 + 1];
		ch[(i__ + (k + j * ch_dim3) * ch_dim2) * ch_dim1 + 1] = c1[(
			i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] + 
			c1[(i__ - 1 + (k + jc * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1];
		ch[(i__ + (k + jc * ch_dim3) * ch_dim2) * ch_dim1 + 1] = c1[(
			i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] - 
			c1[(i__ - 1 + (k + jc * c1_dim3) * c1_dim2) * c1_dim1 
			+ 1];
/* L129: */
	    }
/* L130: */
	}
/* L131: */
    }
L132:
    if (*ido == 1) {
	return 0;
    }
    i__1 = *idl1;
    for (ik = 1; ik <= i__1; ++ik) {
	c2[(ik + c2_dim2) * c2_dim1 + 1] = ch2[(ik + ch2_dim2) * ch2_dim1 + 1]
		;
/* L133: */
    }
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    c1[((k + j * c1_dim3) * c1_dim2 + 1) * c1_dim1 + 1] = ch[((k + j *
		     ch_dim3) * ch_dim2 + 1) * ch_dim1 + 1];
/* L134: */
	}
/* L135: */
    }
    if (nbd > *l1) {
	goto L139;
    }
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	idij = is;
	i__2 = *ido;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    idij += 2;
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] = 
			wa[idij - 1] * ch[(i__ - 1 + (k + j * ch_dim3) * 
			ch_dim2) * ch_dim1 + 1] - wa[idij] * ch[(i__ + (k + j 
			* ch_dim3) * ch_dim2) * ch_dim1 + 1];
		c1[(i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] = wa[
			idij - 1] * ch[(i__ + (k + j * ch_dim3) * ch_dim2) * 
			ch_dim1 + 1] + wa[idij] * ch[(i__ - 1 + (k + j * 
			ch_dim3) * ch_dim2) * ch_dim1 + 1];
/* L136: */
	    }
/* L137: */
	}
/* L138: */
    }
    goto L143;
L139:
    is = -(*ido);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	is += *ido;
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    idij = is;
	    i__3 = *ido;
	    for (i__ = 3; i__ <= i__3; i__ += 2) {
		idij += 2;
		c1[(i__ - 1 + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] = 
			wa[idij - 1] * ch[(i__ - 1 + (k + j * ch_dim3) * 
			ch_dim2) * ch_dim1 + 1] - wa[idij] * ch[(i__ + (k + j 
			* ch_dim3) * ch_dim2) * ch_dim1 + 1];
		c1[(i__ + (k + j * c1_dim3) * c1_dim2) * c1_dim1 + 1] = wa[
			idij - 1] * ch[(i__ + (k + j * ch_dim3) * ch_dim2) * 
			ch_dim1 + 1] + wa[idij] * ch[(i__ - 1 + (k + j * 
			ch_dim3) * ch_dim2) * ch_dim1 + 1];
/* L140: */
	    }
/* L141: */
	}
/* L142: */
    }
L143:
    return 0;
} /* r1fgkb_ */

