/* cfft1b.f -- translated by f2c (version 20100827).
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
static integer c__6 = 6;
static integer c__8 = 8;

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

/* Subroutine */ int cfft1b_(integer *n, integer *inc, complex *c__, integer *
	lenc, real *wsave, integer *lensav, real *work, integer *lenwrk, 
	integer *ier)
{
    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer iw1;
    extern /* Subroutine */ int c1fm1b_(integer *, integer *, complex *, real 
	    *, real *, real *, real *), xerfft_(char *, integer *, ftnlen);


    /* Parameter adjustments */
    --c__;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;

    if (*lenc < *inc * (*n - 1) + 1) {
	*ier = 1;
	xerfft_("CFFT1B ", &c__4, (ftnlen)7);
    } else if (*lensav < (*n << 1) + (integer) (log((real) (*n)) / log(2.f)) 
	    + 4) {
	*ier = 2;
	xerfft_("CFFT1B ", &c__6, (ftnlen)7);
    } else if (*lenwrk < *n << 1) {
	*ier = 3;
	xerfft_("CFFT1B ", &c__8, (ftnlen)7);
    }

    if (*n == 1) {
	return 0;
    }

    iw1 = *n + *n + 1;
    c1fm1b_(n, inc, &c__[1], &work[1], &wsave[1], &wsave[iw1], &wsave[iw1 + 1]
	    );
    return 0;
} /* cfft1b_ */

