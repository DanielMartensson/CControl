/* cfft2i.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int cfft2i_(integer *l, integer *m, real *wsave, integer *
	lensav, integer *ier)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer ier1;
    extern /* Subroutine */ int cfftmi_(integer *, real *, integer *, integer 
	    *), xerfft_(char *, integer *, ftnlen);


/* Initialize error return */

    /* Parameter adjustments */
    --wsave;

    /* Function Body */
    *ier = 0;

    if (*lensav < (*l << 1) + (integer) (log((real) (*l)) / log(2.f)) + (*m <<
	     1) + (integer) (log((real) (*m)) / log(2.f)) + 8) {
	*ier = 2;
	xerfft_("CFFT2I", &c__4, (ftnlen)6);
	goto L100;
    }

    i__1 = (*l << 1) + (integer) (log((real) (*l)) / log(2.f)) + 4;
    cfftmi_(l, &wsave[1], &i__1, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("CFFT2I", &c_n5, (ftnlen)6);
	goto L100;
    }
    i__1 = (*m << 1) + (integer) (log((real) (*m)) / log(2.f)) + 4;
    cfftmi_(m, &wsave[(*l << 1) + (integer) (log((real) (*l)) / log(2.f)) + 3]
	    , &i__1, &ier1);
    if (ier1 != 0) {
	*ier = 20;
	xerfft_("CFFT2I", &c_n5, (ftnlen)6);
    }

L100:
    return 0;
} /* cfft2i_ */

