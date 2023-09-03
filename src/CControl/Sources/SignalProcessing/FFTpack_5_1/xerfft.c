/* xerfft.f -- translated by f2c (version 20100827).
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

/* Subroutine */ int xerfft_(char *srname, integer *info, ftnlen srname_len)
{
    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Fortran I/O blocks */
    static cilist io___1 = { 0, 6, 0, "(A,A,A,I3,A)", 0 };
    static cilist io___2 = { 0, 6, 0, "(A,A,A,A)", 0 };
    static cilist io___3 = { 0, 6, 0, "(A,A,A,A)", 0 };
    static cilist io___4 = { 0, 6, 0, "(A,A,A,A)", 0 };
    static cilist io___5 = { 0, 6, 0, "(A,A,A,A)", 0 };
    static cilist io___6 = { 0, 6, 0, "(A,A,A,A)", 0 };



/*     .. Scalar Arguments .. */

/*     .. */

/*  Purpose */
/*  ======= */

/*  XERFFT  is an error handler for library FFTPACK version 5.1 routines. */
/*  It is called by an FFTPACK 5.1 routine if an input parameter has an */
/*  invalid value.  A message is printed and execution stops. */

/*  Installers may consider modifying the STOP statement in order to */
/*  call system-specific exception-handling facilities. */

/*  Arguments */
/*  ========= */

/*  SRNAME  (input) CHARACTER*6 */
/*          The name of the routine which called XERFFT. */

/*  INFO    (input) INTEGER */
/*          When a single  invalid parameter in the parameter list of */
/*          the calling routine has been detected, INFO is the position */
/*          of that parameter.  In the case when an illegal combination */
/*          of LOT, JUMP, N, and INC has been detected, the calling */
/*          subprogram calls XERFFT with INFO = -1. */

/* ===================================================================== */

/*     .. Executable Statements .. */

    if (*info >= 1) {
	s_wsfe(&io___1);
	do_fio(&c__1, " ** On entry to ", (ftnlen)16);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " parameter number ", (ftnlen)18);
	do_fio(&c__1, (char *)&(*info), (ftnlen)sizeof(integer));
	do_fio(&c__1, " had an illegal value", (ftnlen)21);
	e_wsfe();
    } else if (*info == -1) {
	s_wsfe(&io___2);
	do_fio(&c__1, " ** On entry to ", (ftnlen)16);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " parameters LOT, JUMP, N and INC are inconsistent", (
		ftnlen)49);
	e_wsfe();
    } else if (*info == -2) {
	s_wsfe(&io___3);
	do_fio(&c__1, " ** On entry to ", (ftnlen)16);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " parameter L is greater than LDIM", (ftnlen)33);
	e_wsfe();
    } else if (*info == -3) {
	s_wsfe(&io___4);
	do_fio(&c__1, " ** On entry to ", (ftnlen)16);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " parameter M is greater than MDIM", (ftnlen)33);
	e_wsfe();
    } else if (*info == -5) {
	s_wsfe(&io___5);
	do_fio(&c__1, " ** Within ", (ftnlen)11);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " input error returned by lower level routine", (ftnlen)
		44);
	e_wsfe();
    } else if (*info == -6) {
	s_wsfe(&io___6);
	do_fio(&c__1, " ** On entry to ", (ftnlen)16);
	do_fio(&c__1, srname, (ftnlen)6);
	do_fio(&c__1, " parameter LDIM is less than 2*(L/2+1)", (ftnlen)38);
	e_wsfe();
    }

    s_stop("", (ftnlen)0);

/*     End of XERFFT */

    return 0;
} /* xerfft_ */

