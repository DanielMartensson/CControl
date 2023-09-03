/* cmfgkb.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cmfgkb_(integer *lot, integer *ido, integer *ip, integer 
	*l1, integer *lid, integer *na, real *cc, real *cc1, integer *im1, 
	integer *in1, real *ch, real *ch1, integer *im2, integer *in2, real *
	wa)
{
    /* System generated locals */
    integer ch_dim2, ch_dim3, ch_dim4, ch_offset, cc_dim2, cc_dim3, cc_dim4, 
	    cc_offset, cc1_dim2, cc1_dim3, cc1_offset, ch1_dim2, ch1_dim3, 
	    ch1_offset, wa_dim1, wa_dim2, wa_offset, i__1, i__2, i__3, i__4, 
	    i__5;

    /* Local variables */
    static integer i__, j, k, l, m1, m2, jc, lc, ki, m1d, m2s;
    static real wai, war;
    static integer ipp2, idlj, ipph;
    static real chold1, chold2;


/* FFTPACK 5.0 auxiliary routine */

    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_dim2 = *ip - 1;
    wa_offset = 1 + wa_dim1 * (1 + wa_dim2);
    wa -= wa_offset;
    cc1_dim2 = *in1;
    cc1_dim3 = *lid;
    cc1_offset = 1 + 2 * (1 + cc1_dim2 * (1 + cc1_dim3));
    cc1 -= cc1_offset;
    cc_dim2 = *in1;
    cc_dim3 = *l1;
    cc_dim4 = *ip;
    cc_offset = 1 + 2 * (1 + cc_dim2 * (1 + cc_dim3 * (1 + cc_dim4)));
    cc -= cc_offset;
    ch1_dim2 = *in2;
    ch1_dim3 = *lid;
    ch1_offset = 1 + 2 * (1 + ch1_dim2 * (1 + ch1_dim3));
    ch1 -= ch1_offset;
    ch_dim2 = *in2;
    ch_dim3 = *l1;
    ch_dim4 = *ido;
    ch_offset = 1 + 2 * (1 + ch_dim2 * (1 + ch_dim3 * (1 + ch_dim4)));
    ch -= ch_offset;

    /* Function Body */
    m1d = (*lot - 1) * *im1 + 1;
    m2s = 1 - *im2;
    ipp2 = *ip + 2;
    ipph = (*ip + 1) / 2;
    i__1 = *lid;
    for (ki = 1; ki <= i__1; ++ki) {
	m2 = m2s;
	i__2 = m1d;
	i__3 = *im1;
	for (m1 = 1; i__3 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__3) {
	    m2 += *im2;
	    ch1[(m2 + (ki + ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(m1 + (ki + 
		    cc1_dim3) * cc1_dim2 << 1) + 1];
	    ch1[(m2 + (ki + ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(m1 + (ki + 
		    cc1_dim3) * cc1_dim2 << 1) + 2];
/* L110: */
	}
    }
    i__3 = ipph;
    for (j = 2; j <= i__3; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    m2 = m2s;
	    i__1 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__1 : m1 <= i__1; m1 += i__4) {
		m2 += *im2;
		ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(m1 
			+ (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] + cc1[(m1 
			+ (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
		ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(
			m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] - cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
		ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(m1 
			+ (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] + cc1[(m1 
			+ (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2];
		ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(
			m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] - cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2];
/* L112: */
	    }
	}
/* L111: */
    }
    i__3 = ipph;
    for (j = 2; j <= i__3; ++j) {
	i__4 = *lid;
	for (ki = 1; ki <= i__4; ++ki) {
	    m2 = m2s;
	    i__1 = m1d;
	    i__2 = *im1;
	    for (m1 = 1; i__2 < 0 ? m1 >= i__1 : m1 <= i__1; m1 += i__2) {
		m2 += *im2;
		cc1[(m1 + (ki + cc1_dim3) * cc1_dim2 << 1) + 1] += ch1[(m2 + (
			ki + j * ch1_dim3) * ch1_dim2 << 1) + 1];
		cc1[(m1 + (ki + cc1_dim3) * cc1_dim2 << 1) + 2] += ch1[(m2 + (
			ki + j * ch1_dim3) * ch1_dim2 << 1) + 2];
/* L117: */
	    }
	}
/* L118: */
    }
    i__3 = ipph;
    for (l = 2; l <= i__3; ++l) {
	lc = ipp2 - l;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    m2 = m2s;
	    i__1 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__1 : m1 <= i__1; m1 += i__4) {
		m2 += *im2;
		cc1[(m1 + (ki + l * cc1_dim3) * cc1_dim2 << 1) + 1] = ch1[(m2 
			+ (ki + ch1_dim3) * ch1_dim2 << 1) + 1] + wa[(l - 1 + 
			wa_dim2) * wa_dim1 + 1] * ch1[(m2 + (ki + (ch1_dim3 <<
			 1)) * ch1_dim2 << 1) + 1];
		cc1[(m1 + (ki + lc * cc1_dim3) * cc1_dim2 << 1) + 1] = wa[(l 
			- 1 + (wa_dim2 << 1)) * wa_dim1 + 1] * ch1[(m2 + (ki 
			+ *ip * ch1_dim3) * ch1_dim2 << 1) + 1];
		cc1[(m1 + (ki + l * cc1_dim3) * cc1_dim2 << 1) + 2] = ch1[(m2 
			+ (ki + ch1_dim3) * ch1_dim2 << 1) + 2] + wa[(l - 1 + 
			wa_dim2) * wa_dim1 + 1] * ch1[(m2 + (ki + (ch1_dim3 <<
			 1)) * ch1_dim2 << 1) + 2];
		cc1[(m1 + (ki + lc * cc1_dim3) * cc1_dim2 << 1) + 2] = wa[(l 
			- 1 + (wa_dim2 << 1)) * wa_dim1 + 1] * ch1[(m2 + (ki 
			+ *ip * ch1_dim3) * ch1_dim2 << 1) + 2];
/* L113: */
	    }
	}
	i__4 = ipph;
	for (j = 3; j <= i__4; ++j) {
	    jc = ipp2 - j;
	    idlj = (l - 1) * (j - 1) % *ip;
	    war = wa[(idlj + wa_dim2) * wa_dim1 + 1];
	    wai = wa[(idlj + (wa_dim2 << 1)) * wa_dim1 + 1];
	    i__1 = *lid;
	    for (ki = 1; ki <= i__1; ++ki) {
		m2 = m2s;
		i__2 = m1d;
		i__5 = *im1;
		for (m1 = 1; i__5 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__5) {
		    m2 += *im2;
		    cc1[(m1 + (ki + l * cc1_dim3) * cc1_dim2 << 1) + 1] += 
			    war * ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 
			    1) + 1];
		    cc1[(m1 + (ki + lc * cc1_dim3) * cc1_dim2 << 1) + 1] += 
			    wai * ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 <<
			     1) + 1];
		    cc1[(m1 + (ki + l * cc1_dim3) * cc1_dim2 << 1) + 2] += 
			    war * ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 
			    1) + 2];
		    cc1[(m1 + (ki + lc * cc1_dim3) * cc1_dim2 << 1) + 2] += 
			    wai * ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 <<
			     1) + 2];
/* L114: */
		}
	    }
/* L115: */
	}
/* L116: */
    }
    if (*ido > 1 || *na == 1) {
	goto L136;
    }
    i__3 = ipph;
    for (j = 2; j <= i__3; ++j) {
	jc = ipp2 - j;
	i__4 = *lid;
	for (ki = 1; ki <= i__4; ++ki) {
	    i__5 = m1d;
	    i__2 = *im1;
	    for (m1 = 1; i__2 < 0 ? m1 >= i__5 : m1 <= i__5; m1 += i__2) {
		chold1 = cc1[(m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] 
			- cc1[(m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2]
			;
		chold2 = cc1[(m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] 
			+ cc1[(m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2]
			;
		cc1[(m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] = chold1;
		cc1[(m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2] = cc1[(
			m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] - cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
		cc1[(m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] += cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
		cc1[(m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1] = chold2;
/* L119: */
	    }
	}
/* L120: */
    }
    return 0;
L136:
    i__3 = *lid;
    for (ki = 1; ki <= i__3; ++ki) {
	m2 = m2s;
	i__2 = m1d;
	i__5 = *im1;
	for (m1 = 1; i__5 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__5) {
	    m2 += *im2;
	    ch1[(m2 + (ki + ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(m1 + (ki + 
		    cc1_dim3) * cc1_dim2 << 1) + 1];
	    ch1[(m2 + (ki + ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(m1 + (ki + 
		    cc1_dim3) * cc1_dim2 << 1) + 2];
/* L137: */
	}
    }
    i__5 = ipph;
    for (j = 2; j <= i__5; ++j) {
	jc = ipp2 - j;
	i__2 = *lid;
	for (ki = 1; ki <= i__2; ++ki) {
	    m2 = m2s;
	    i__3 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__3 : m1 <= i__3; m1 += i__4) {
		m2 += *im2;
		ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(m1 
			+ (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] - cc1[(m1 
			+ (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2];
		ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 << 1) + 1] = cc1[(
			m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 1] + cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 2];
		ch1[(m2 + (ki + jc * ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(
			m1 + (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] - cc1[(
			m1 + (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
		ch1[(m2 + (ki + j * ch1_dim3) * ch1_dim2 << 1) + 2] = cc1[(m1 
			+ (ki + j * cc1_dim3) * cc1_dim2 << 1) + 2] + cc1[(m1 
			+ (ki + jc * cc1_dim3) * cc1_dim2 << 1) + 1];
/* L134: */
	    }
	}
/* L135: */
    }
    if (*ido == 1) {
	return 0;
    }
    i__5 = *ido;
    for (i__ = 1; i__ <= i__5; ++i__) {
	i__4 = *l1;
	for (k = 1; k <= i__4; ++k) {
	    m2 = m2s;
	    i__3 = m1d;
	    i__2 = *im1;
	    for (m1 = 1; i__2 < 0 ? m1 >= i__3 : m1 <= i__3; m1 += i__2) {
		m2 += *im2;
		cc[(m1 + (k + (i__ * cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) 
			+ 1] = ch[(m2 + (k + (i__ + ch_dim4) * ch_dim3) * 
			ch_dim2 << 1) + 1];
		cc[(m1 + (k + (i__ * cc_dim4 + 1) * cc_dim3) * cc_dim2 << 1) 
			+ 2] = ch[(m2 + (k + (i__ + ch_dim4) * ch_dim3) * 
			ch_dim2 << 1) + 2];
/* L130: */
	    }
	}
/* L131: */
    }
    i__5 = *ip;
    for (j = 2; j <= i__5; ++j) {
	i__2 = *l1;
	for (k = 1; k <= i__2; ++k) {
	    m2 = m2s;
	    i__3 = m1d;
	    i__4 = *im1;
	    for (m1 = 1; i__4 < 0 ? m1 >= i__3 : m1 <= i__3; m1 += i__4) {
		m2 += *im2;
		cc[(m1 + (k + (j + cc_dim4) * cc_dim3) * cc_dim2 << 1) + 1] = 
			ch[(m2 + (k + (j * ch_dim4 + 1) * ch_dim3) * ch_dim2 
			<< 1) + 1];
		cc[(m1 + (k + (j + cc_dim4) * cc_dim3) * cc_dim2 << 1) + 2] = 
			ch[(m2 + (k + (j * ch_dim4 + 1) * ch_dim3) * ch_dim2 
			<< 1) + 2];
/* L122: */
	    }
	}
/* L123: */
    }
    i__5 = *ip;
    for (j = 2; j <= i__5; ++j) {
	i__4 = *ido;
	for (i__ = 2; i__ <= i__4; ++i__) {
	    i__3 = *l1;
	    for (k = 1; k <= i__3; ++k) {
		m2 = m2s;
		i__2 = m1d;
		i__1 = *im1;
		for (m1 = 1; i__1 < 0 ? m1 >= i__2 : m1 <= i__2; m1 += i__1) {
		    m2 += *im2;
		    cc[(m1 + (k + (j + i__ * cc_dim4) * cc_dim3) * cc_dim2 << 
			    1) + 1] = wa[i__ + (j - 1 + wa_dim2) * wa_dim1] * 
			    ch[(m2 + (k + (i__ + j * ch_dim4) * ch_dim3) * 
			    ch_dim2 << 1) + 1] - wa[i__ + (j - 1 + (wa_dim2 <<
			     1)) * wa_dim1] * ch[(m2 + (k + (i__ + j * 
			    ch_dim4) * ch_dim3) * ch_dim2 << 1) + 2];
		    cc[(m1 + (k + (j + i__ * cc_dim4) * cc_dim3) * cc_dim2 << 
			    1) + 2] = wa[i__ + (j - 1 + wa_dim2) * wa_dim1] * 
			    ch[(m2 + (k + (i__ + j * ch_dim4) * ch_dim3) * 
			    ch_dim2 << 1) + 2] + wa[i__ + (j - 1 + (wa_dim2 <<
			     1)) * wa_dim1] * ch[(m2 + (k + (i__ + j * 
			    ch_dim4) * ch_dim3) * ch_dim2 << 1) + 1];
/* L124: */
		}
	    }
/* L125: */
	}
/* L126: */
    }
    return 0;
} /* cmfgkb_ */

