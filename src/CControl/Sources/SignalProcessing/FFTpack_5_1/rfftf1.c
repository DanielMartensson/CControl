/* rfftf1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int rfftf1_(integer *n, integer *in, real *c__, real *ch, 
	real *wa, real *fac)
{
    /* System generated locals */
    integer c_dim1, c_offset, i__1;

    /* Local variables */
    static integer j, k1, l1, l2, na, kh, nf, ip, nl, iw;
    static real sn;
    static integer ix2, ix3, ix4, ido;
    static real tsn;
    static integer idl1, modn;
    static real tsnm;
    extern /* Subroutine */ int r1f2kf_(integer *, integer *, real *, integer 
	    *, real *, integer *, real *), r1f3kf_(integer *, integer *, real 
	    *, integer *, real *, integer *, real *, real *), r1f4kf_(integer 
	    *, integer *, real *, integer *, real *, integer *, real *, real *
	    , real *), r1f5kf_(integer *, integer *, real *, integer *, real *
	    , integer *, real *, real *, real *, real *), r1fgkf_(integer *, 
	    integer *, integer *, integer *, real *, real *, real *, integer *
	    , real *, real *, integer *, real *);


    /* Parameter adjustments */
    --wa;
    c_dim1 = *in;
    c_offset = 1 + c_dim1;
    c__ -= c_offset;
    --ch;
    --fac;

    /* Function Body */
    nf = fac[2];
    na = 1;
    l2 = *n;
    iw = *n;
    i__1 = nf;
    for (k1 = 1; k1 <= i__1; ++k1) {
	kh = nf - k1;
	ip = fac[kh + 3];
	l1 = l2 / ip;
	ido = *n / l2;
	idl1 = ido * l1;
	iw -= (ip - 1) * ido;
	na = 1 - na;
	if (ip != 4) {
	    goto L102;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	if (na != 0) {
	    goto L101;
	}
	r1f4kf_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2], &wa[ix3]);
	goto L110;
L101:
	r1f4kf_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2], &wa[ix3]);
	goto L110;
L102:
	if (ip != 2) {
	    goto L104;
	}
	if (na != 0) {
	    goto L103;
	}
	r1f2kf_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw]);
	goto L110;
L103:
	r1f2kf_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw]);
	goto L110;
L104:
	if (ip != 3) {
	    goto L106;
	}
	ix2 = iw + ido;
	if (na != 0) {
	    goto L105;
	}
	r1f3kf_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2]);
	goto L110;
L105:
	r1f3kf_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2]);
	goto L110;
L106:
	if (ip != 5) {
	    goto L108;
	}
	ix2 = iw + ido;
	ix3 = ix2 + ido;
	ix4 = ix3 + ido;
	if (na != 0) {
	    goto L107;
	}
	r1f5kf_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2], &wa[ix3], &wa[ix4]);
	goto L110;
L107:
	r1f5kf_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2], &wa[ix3], &wa[ix4]);
	goto L110;
L108:
	if (ido == 1) {
	    na = 1 - na;
	}
	if (na != 0) {
	    goto L109;
	}
	r1fgkf_(&ido, &ip, &l1, &idl1, &c__[c_offset], &c__[c_offset], &c__[
		c_offset], in, &ch[1], &ch[1], &c__1, &wa[iw]);
	na = 1;
	goto L110;
L109:
	r1fgkf_(&ido, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__1, &c__[
		c_offset], &c__[c_offset], in, &wa[iw]);
	na = 0;
L110:
	l2 = l1;
/* L111: */
    }
    sn = 1.f / *n;
    tsn = 2.f / *n;
    tsnm = -tsn;
    modn = *n % 2;
    nl = *n - 2;
    if (modn != 0) {
	nl = *n - 1;
    }
    if (na != 0) {
	goto L120;
    }
    c__[c_dim1 + 1] = sn * ch[1];
    i__1 = nl;
    for (j = 2; j <= i__1; j += 2) {
	c__[j * c_dim1 + 1] = tsn * ch[j];
	c__[(j + 1) * c_dim1 + 1] = tsnm * ch[j + 1];
/* L118: */
    }
    if (modn != 0) {
	return 0;
    }
    c__[*n * c_dim1 + 1] = sn * ch[*n];
    return 0;
L120:
    c__[c_dim1 + 1] = sn * c__[c_dim1 + 1];
    i__1 = nl;
    for (j = 2; j <= i__1; j += 2) {
	c__[j * c_dim1 + 1] = tsn * c__[j * c_dim1 + 1];
	c__[(j + 1) * c_dim1 + 1] = tsnm * c__[(j + 1) * c_dim1 + 1];
/* L122: */
    }
    if (modn != 0) {
	return 0;
    }
    c__[*n * c_dim1 + 1] = sn * c__[*n * c_dim1 + 1];
    return 0;
} /* rfftf1_ */

