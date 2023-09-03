/* rfft2i.f -- translated by f2c (version 20100827).
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

static integer c__4 = 4;
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

/* Subroutine */ int rfft2i_(integer *l, integer *m, real *wsave, integer *
	lensav, integer *ier)
{
    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer ier1, mmsav, lwsav, mwsav;
    extern /* Subroutine */ int cfftmi_(integer *, real *, integer *, integer 
	    *), rfftmi_(integer *, real *, integer *, integer *), xerfft_(
	    char *, integer *, ftnlen);


/* INITIALIZE IER */

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    *ier = 0;

/* VERIFY LENSAV */

    lwsav = *l + (integer) (log((real) (*l)) / log(2.f)) + 4;
    mwsav = (*m << 1) + (integer) (log((real) (*m)) / log(2.f)) + 4;
    mmsav = *m + (integer) (log((real) (*m)) / log(2.f)) + 4;
    if (*lensav < lwsav + mwsav + mmsav) {
	*ier = 2;
	xerfft_("RFFT2I", &c__4, (ftnlen)6);
	goto L100;
    }

    rfftmi_(l, &wsave[1], &lwsav, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("RFFT2I", &c_n5, (ftnlen)6);
	goto L100;
    }
    cfftmi_(m, &wsave[lwsav + 1], &mwsav, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("RFFT2I", &c_n5, (ftnlen)6);
    }

    rfftmi_(m, &wsave[lwsav + mwsav + 1], &mmsav, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("RFFT2I", &c_n5, (ftnlen)6);
	goto L100;
    }

L100:
    return 0;
} /* rfft2i_ */

