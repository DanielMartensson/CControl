/* rfftmb.f -- translated by f2c (version 20100827).
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
static integer c__10 = 10;
static integer c_n1 = -1;

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

/* Subroutine */ int rfftmb_(integer *lot, integer *jump, integer *n, integer 
	*inc, real *r__, integer *lenr, real *wsave, integer *lensav, real *
	work, integer *lenwrk, integer *ier)
{
    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    extern /* Subroutine */ int mrftb1_(integer *, integer *, integer *, 
	    integer *, real *, real *, real *, real *);
    extern logical xercon_(integer *, integer *, integer *, integer *);
    extern /* Subroutine */ int xerfft_(char *, integer *, ftnlen);


    /* Parameter adjustments */
    --r__;
    --wsave;
    --work;

    /* Function Body */
    *ier = 0;

    if (*lenr < (*lot - 1) * *jump + *inc * (*n - 1) + 1) {
	*ier = 1;
	xerfft_("RFFTMB ", &c__6, (ftnlen)7);
    } else if (*lensav < *n + (integer) (log((real) (*n)) / log(2.f)) + 4) {
	*ier = 2;
	xerfft_("RFFTMB ", &c__8, (ftnlen)7);
    } else if (*lenwrk < *lot * *n) {
	*ier = 3;
	xerfft_("RFFTMB ", &c__10, (ftnlen)7);
    } else if (! xercon_(inc, jump, n, lot)) {
	*ier = 4;
	xerfft_("RFFTMB ", &c_n1, (ftnlen)7);
    }

    if (*n == 1) {
	return 0;
    }

    mrftb1_(lot, jump, n, inc, &r__[1], &work[1], &wsave[1], &wsave[*n + 1]);
    return 0;
} /* rfftmb_ */

