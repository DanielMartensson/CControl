/* xercon.f -- translated by f2c (version 20100827).
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

logical xercon_(integer *inc, integer *jump, integer *n, integer *lot)
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    static integer i__, j, lcm, jnew;


/*     Definition: positive integers INC, JUMP, N and LOT are consistent */
/*                                                            ---------- */
/*     if I1*INC + J1*JUMP = I2*INC + J2*JUMP for I1,I2 < N and J1,J2 */
/*     < LOT implies I1=I2 and J1=J2. */

/*     For multiple FFTs to execute correctly, input parameters INC, */
/*     JUMP, N and LOT must be consistent ... otherwise at least one */
/*     array element mistakenly is transformed more than once. */

/*     XERCON = .TRUE. if and only if INC, JUMP, N and LOT are */
/*     consistent. */

/*     ------------------------------------------------------------------ */

/*     Compute I = greatest common divisor (INC, JUMP) */

    i__ = *inc;
    j = *jump;
L10:
    if (j != 0) {
	jnew = i__ % j;
	i__ = j;
	j = jnew;
	goto L10;
    }

/* Compute LCM = least common multiple (INC, JUMP) */

    lcm = *inc * *jump / i__;

/* Check consistency of INC, JUMP, N, LOT */

    if (lcm <= (*n - 1) * *inc && lcm <= (*lot - 1) * *jump) {
	ret_val = FALSE_;
    } else {
	ret_val = TRUE_;
    }

    return ret_val;
} /* xercon_ */

