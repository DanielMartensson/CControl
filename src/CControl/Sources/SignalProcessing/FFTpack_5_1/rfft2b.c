/* rfft2b.f -- translated by f2c (version 20100827).
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

static integer c__6 = 6;
static integer c__8 = 8;
static integer c_n6 = -6;
static integer c__1 = 1;
static integer c_n5 = -5;

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

/* Subroutine */ int rfft2b_(integer *ldim, integer *l, integer *m, real *r__,
	 real *wsave, integer *lensav, real *work, integer *lenwrk, integer *
	ier)
{
    /* System generated locals */
    integer r_dim1, r_offset, i__1, i__2, i__3;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer i__, j;
    extern /* Subroutine */ int r2w_(integer *, integer *, integer *, integer 
	    *, real *, real *), w2r_(integer *, integer *, integer *, integer 
	    *, real *, real *);
    static integer ldh, ldw, ldx, ier1, modl, modm, mmsav, lwsav, mwsav;
    extern /* Subroutine */ int cfftmb_(integer *, integer *, integer *, 
	    integer *, real *, integer *, real *, integer *, real *, integer *
	    , integer *), rfftmb_(integer *, integer *, integer *, integer *, 
	    real *, integer *, real *, integer *, real *, integer *, integer *
	    ), xerfft_(char *, integer *, ftnlen);



/* INITIALIZE IER */

    /* Parameter adjustments */
    r_dim1 = *ldim;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;

/* VERIFY LENSAV */

    lwsav = *l + (integer) (log((real) (*l)) / log(2.f)) + 4;
    mwsav = (*m << 1) + (integer) (log((real) (*m)) / log(2.f)) + 4;
    mmsav = *m + (integer) (log((real) (*m)) / log(2.f)) + 4;
    modl = *l % 2;
    modm = *m % 2;

    if (*lensav < lwsav + mwsav + mmsav) {
	*ier = 2;
	xerfft_("RFFT2F", &c__6, (ftnlen)6);
	goto L100;
    }

/* VERIFY LENWRK */

    if (*lenwrk < (*l + 1) * *m) {
	*ier = 3;
	xerfft_("RFFT2F", &c__8, (ftnlen)6);
	goto L100;
    }

/* VERIFY LDIM IS AS BIG AS L */

    if (*ldim < *l) {
	*ier = 5;
	xerfft_("RFFT2F", &c_n6, (ftnlen)6);
	goto L100;
    }

/* TRANSFORM SECOND DIMENSION OF ARRAY */

    i__1 = ((*m + 1) / 2 << 1) - 1;
    for (j = 2; j <= i__1; ++j) {
	r__[j * r_dim1 + 1] += r__[j * r_dim1 + 1];
    }
    i__1 = *m;
    for (j = 3; j <= i__1; j += 2) {
	r__[j * r_dim1 + 1] = -r__[j * r_dim1 + 1];
    }
    i__1 = *m * *ldim;
    rfftmb_(&c__1, &c__1, m, ldim, &r__[r_offset], &i__1, &wsave[lwsav + 
	    mwsav + 1], &mmsav, &work[1], lenwrk, &ier1);
    ldh = (*l + 1) / 2;
    if (ldh > 1) {
	ldw = ldh + ldh;

/*     R AND WORK ARE SWITCHED BECAUSE THE THE FIRST DIMENSION */
/*     OF THE INPUT TO COMPLEX CFFTMF MUST BE EVEN. */

	r2w_(ldim, &ldw, l, m, &r__[r_offset], &work[1]);
	i__1 = ldh - 1;
	i__2 = ldh * *m;
	i__3 = *l * *m;
	cfftmb_(&i__1, &c__1, m, &ldh, &work[2], &i__2, &wsave[lwsav + 1], &
		mwsav, &r__[r_offset], &i__3, &ier1);
	if (ier1 != 0) {
	    *ier = 20;
	    xerfft_("RFFT2B", &c_n5, (ftnlen)6);
	    goto L100;
	}
	w2r_(ldim, &ldw, l, m, &r__[r_offset], &work[1]);
    }

    if (modl == 0) {
	i__1 = ((*m + 1) / 2 << 1) - 1;
	for (j = 2; j <= i__1; ++j) {
	    r__[*l + j * r_dim1] += r__[*l + j * r_dim1];
	}
	i__1 = *m;
	for (j = 3; j <= i__1; j += 2) {
	    r__[*l + j * r_dim1] = -r__[*l + j * r_dim1];
	}
	i__1 = *m * *ldim;
	rfftmb_(&c__1, &c__1, m, ldim, &r__[*l + r_dim1], &i__1, &wsave[lwsav 
		+ mwsav + 1], &mmsav, &work[1], lenwrk, &ier1);
    }

/*     PRINT*, 'BACKWARD TRANSFORM IN THE J DIRECTION' */
/*     DO I=1,L */
/*       PRINT*, (R(I,J),J=1,M) */
/*     END DO */

/* TRANSFORM FIRST DIMENSION OF ARRAY */

    ldx = ((*l + 1) / 2 << 1) - 1;
    i__1 = ldx;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = *m;
	for (j = 1; j <= i__2; ++j) {
	    r__[i__ + j * r_dim1] += r__[i__ + j * r_dim1];
	}
    }
    i__1 = *m;
    for (j = 1; j <= i__1; ++j) {
	i__2 = ldx;
	for (i__ = 3; i__ <= i__2; i__ += 2) {
	    r__[i__ + j * r_dim1] = -r__[i__ + j * r_dim1];
	}
    }
    i__1 = *m * *ldim;
    i__2 = *l + (integer) (log((real) (*l)) / log(2.f)) + 4;
    rfftmb_(m, ldim, l, &c__1, &r__[r_offset], &i__1, &wsave[1], &i__2, &work[
	    1], lenwrk, &ier1);


/*     PRINT*, 'BACKWARD TRANSFORM IN THE I DIRECTION' */
/*     DO I=1,L */
/*       PRINT*, (R(I,J),J=1,M) */
/*     END DO */

    if (ier1 != 0) {
	*ier = 20;
	xerfft_("RFFT2F", &c_n5, (ftnlen)6);
	goto L100;
    }

    if (ier1 != 0) {
	*ier = 20;
	xerfft_("RFFT2F", &c_n5, (ftnlen)6);
	goto L100;
    }

L100:

    return 0;
} /* rfft2b_ */

