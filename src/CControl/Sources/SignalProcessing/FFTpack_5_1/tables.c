/* tables.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int tables_(integer *ido, integer *ip, real *wa)
{
    /* System generated locals */
    integer wa_dim1, wa_dim2, wa_offset, i__1, i__2;

    /* Builtin functions */
    double atan(doublereal), cos(doublereal), sin(doublereal);

    /* Local variables */
    static integer i__, j;
    static real tpi, arg1, arg2, arg3, arg4, argz;


    /* Parameter adjustments */
    wa_dim1 = *ido;
    wa_dim2 = *ip - 1;
    wa_offset = 1 + wa_dim1 * (1 + wa_dim2);
    wa -= wa_offset;

    /* Function Body */
    tpi = atan(1.f) * 8.f;
    argz = tpi / (real) (*ip);
    arg1 = tpi / (real) (*ido * *ip);
    i__1 = *ip;
    for (j = 2; j <= i__1; ++j) {
	arg2 = (real) (j - 1) * arg1;
	i__2 = *ido;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    arg3 = (real) (i__ - 1) * arg2;
	    wa[i__ + (j - 1 + wa_dim2) * wa_dim1] = cos(arg3);
	    wa[i__ + (j - 1 + (wa_dim2 << 1)) * wa_dim1] = sin(arg3);
/* L100: */
	}
	if (*ip <= 5) {
	    goto L110;
	}
	arg4 = (real) (j - 1) * argz;
	wa[(j - 1 + wa_dim2) * wa_dim1 + 1] = cos(arg4);
	wa[(j - 1 + (wa_dim2 << 1)) * wa_dim1 + 1] = sin(arg4);
L110:
	;
    }
    return 0;
} /* tables_ */

