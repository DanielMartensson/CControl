/* rfftb1.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int rfftb1_(integer *n, integer *in, real *c__, real *ch, 
	real *wa, real *fac)
{
    /* System generated locals */
    integer c_dim1, c_offset, i__1;

    /* Local variables */
    static integer j, k1, l1, l2, na, nf, ip, nl, iw, ix2, ix3, ix4, ido, 
	    idl1;
    static real half;
    static integer modn;
    static real halfm;
    extern /* Subroutine */ int r1f2kb_(integer *, integer *, real *, integer 
	    *, real *, integer *, real *), r1f4kb_(integer *, integer *, real 
	    *, integer *, real *, integer *, real *, real *, real *), r1f3kb_(
	    integer *, integer *, real *, integer *, real *, integer *, real *
	    , real *), r1f5kb_(integer *, integer *, real *, integer *, real *
	    , integer *, real *, real *, real *, real *), r1fgkb_(integer *, 
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
    ch[1] = c__[c_dim1 + 1];
    ch[*n] = c__[*n * c_dim1 + 1];
    i__1 = nl;
    for (j = 2; j <= i__1; j += 2) {
	ch[j] = half * c__[j * c_dim1 + 1];
	ch[j + 1] = halfm * c__[(j + 1) * c_dim1 + 1];
/* L118: */
    }
    goto L124;
L120:
    i__1 = nl;
    for (j = 2; j <= i__1; j += 2) {
	c__[j * c_dim1 + 1] = half * c__[j * c_dim1 + 1];
	c__[(j + 1) * c_dim1 + 1] = halfm * c__[(j + 1) * c_dim1 + 1];
/* L122: */
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
	r1f4kb_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2], &wa[ix3]);
	goto L102;
L101:
	r1f4kb_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2], &wa[ix3]);
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
	r1f2kb_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw]);
	goto L105;
L104:
	r1f2kb_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw]);
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
	r1f3kb_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2]);
	goto L108;
L107:
	r1f3kb_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2]);
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
	r1f5kb_(&ido, &l1, &c__[c_offset], in, &ch[1], &c__1, &wa[iw], &wa[
		ix2], &wa[ix3], &wa[ix4]);
	goto L111;
L110:
	r1f5kb_(&ido, &l1, &ch[1], &c__1, &c__[c_offset], in, &wa[iw], &wa[
		ix2], &wa[ix3], &wa[ix4]);
L111:
	na = 1 - na;
	goto L115;
L112:
	if (na != 0) {
	    goto L113;
	}
	r1fgkb_(&ido, &ip, &l1, &idl1, &c__[c_offset], &c__[c_offset], &c__[
		c_offset], in, &ch[1], &ch[1], &c__1, &wa[iw]);
	goto L114;
L113:
	r1fgkb_(&ido, &ip, &l1, &idl1, &ch[1], &ch[1], &ch[1], &c__1, &c__[
		c_offset], &c__[c_offset], in, &wa[iw]);
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
} /* rfftb1_ */

