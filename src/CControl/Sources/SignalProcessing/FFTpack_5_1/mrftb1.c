/* mrftb1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int mrftb1_(integer *m, integer *im, integer *n, integer *in,
	 real *c__, real *ch, real *wa, real *fac)
{
    /* System generated locals */
    integer ch_dim1, ch_offset, c_dim1, c_offset, i__1, i__2;

    /* Local variables */
    static integer i__, j, k1, l1, l2, m2, na, nf, ip, nl, iw, ix2, ix3, ix4, 
	    ido, idl1;
    static real half;
    static integer modn;
    static real halfm;
    extern /* Subroutine */ int mradb2_(integer *, integer *, integer *, real 
	    *, integer *, integer *, real *, integer *, integer *, real *), 
	    mradb3_(integer *, integer *, integer *, real *, integer *, 
	    integer *, real *, integer *, integer *, real *, real *), mradb4_(
	    integer *, integer *, integer *, real *, integer *, integer *, 
	    real *, integer *, integer *, real *, real *, real *), mradb5_(
	    integer *, integer *, integer *, real *, integer *, integer *, 
	    real *, integer *, integer *, real *, real *, real *, real *), 
	    mradbg_(integer *, integer *, integer *, integer *, integer *, 
	    real *, real *, real *, integer *, integer *, real *, real *, 
	    integer *, integer *, real *);


    /* Parameter adjustments */
    ch_dim1 = *m;
    ch_offset = 1 + ch_dim1;
    ch -= ch_offset;
    --wa;
    c_dim1 = *in;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --fac;

    /* Function Body */
    nf = fac[2];
    na = 0;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = fac[k1 + 2];
	na = 1 - na;
	if (ip <= 5) {
	    goto L10;
	}
	if (k1 == nf) {
	    goto L10;
	}
	na = 1 - na;
L10:
	;
    }
    half = .5f;
    halfm = -.5f;
    modn = *n % 2;
    nl = *n - 2;
    if (modn != 0) {
	nl = *n - 1;
    }
    if (na == 0) {
	goto L120;
    }
    m2 = 1 - *im;
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	m2 += *im;
	ch[i__ + ch_dim1] = c__[m2 + c_dim1];
	ch[i__ + *n * ch_dim1] = c__[m2 + *n * c_dim1];
/* L117: */
    }
    i__1 = nl;
    for (j = 2; j <= i__1; j += 2) {
	m2 = 1 - *im;
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    m2 += *im;
	    ch[i__ + j * ch_dim1] = half * c__[m2 + j * c_dim1];
	    ch[i__ + (j + 1) * ch_dim1] = halfm * c__[m2 + (j + 1) * c_dim1];
/* L118: */
	}
    }
    goto L124;
L120:
    i__2 = nl;
    for (j = 2; j <= i__2; j += 2) {
	m2 = 1 - *im;
	i__1 = *m;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    m2 += *im;
	    c__[m2 + j * c_dim1] = half * c__[m2 + j * c_dim1];
	    c__[m2 + (j + 1) * c_dim1] = halfm * c__[m2 + (j + 1) * c_dim1];
/* L122: */
	}
    }
L124:
    l1 = 1;
    iw = 1;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	ip = fac[k1 + 2];
	l2 = ip * l1;
	ido = *n / l2;
	idl1 = ido * l1;
	if (ip != 4) {
	    goto L103;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	if (na != 0) {
	    goto L101;
	}
	mradb4_(m, &ido, &l1, &c__[c_offset], im, in, &ch[ch_offset], &c__1, 
		m, &wa[iw], &wa[ix2], &wa[ix3]);
	goto L102;
L101:
	mradb4_(m, &ido, &l1, &ch[ch_offset], &c__1, m, &c__[c_offset], im, 
		in, &wa[iw], &wa[ix2], &wa[ix3]);
L102:
	na = 1 - na;
	goto L115;
L103:
	if (ip != 2) {
	    goto L106;
	}
	if (na != 0) {
	    goto L104;
	}
	mradb2_(m, &ido, &l1, &c__[c_offset], im, in, &ch[ch_offset], &c__1, 
		m, &wa[iw]);
	goto L105;
L104:
	mradb2_(m, &ido, &l1, &ch[ch_offset], &c__1, m, &c__[c_offset], im, 
		in, &wa[iw]);
L105:
	na = 1 - na;
	goto L115;
L106:
	if (ip != 3) {
	    goto L109;
	}
	ix2 = iw + ido;
	if (na != 0) {
	    goto L107;
	}
	mradb3_(m, &ido, &l1, &c__[c_offset], im, in, &ch[ch_offset], &c__1, 
		m, &wa[iw], &wa[ix2]);
	goto L108;
L107:
	mradb3_(m, &ido, &l1, &ch[ch_offset], &c__1, m, &c__[c_offset], im, 
		in, &wa[iw], &wa[ix2]);
L108:
	na = 1 - na;
	goto L115;
L109:
	if (ip != 5) {
	    goto L112;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	ix4 = ix3 + ido;
	if (na != 0) {
	    goto L110;
	}
	mradb5_(m, &ido, &l1, &c__[c_offset], im, in, &ch[ch_offset], &c__1, 
		m, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
	goto L111;
L110:
	mradb5_(m, &ido, &l1, &ch[ch_offset], &c__1, m, &c__[c_offset], im, 
		in, &wa[iw], &wa[ix2], &wa[ix3], &wa[ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	mradbg_(m, &ido, &ip, &l1, &idl1, &c__[c_offset], &c__[c_offset], &
		c__[c_offset], im, in, &ch[ch_offset], &ch[ch_offset], &c__1, 
		m, &wa[iw]);
	goto L114;
L113:
	mradbg_(m, &ido, &ip, &l1, &idl1, &ch[ch_offset], &ch[ch_offset], &ch[
		ch_offset], &c__1, m, &c__[c_offset], &c__[c_offset], im, in, 
		&wa[iw]);
L114:
	if (ido == 1) {
	    na = 1 - na;
	}
L115:
	l1 = l2;
	iw += (ip - 1) * ido;
/* L116: */
    }
    return 0;
} /* mrftb1_ */

