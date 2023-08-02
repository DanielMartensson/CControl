/* slasq3.f -- translated by f2c (version 20061008).
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
#include "blaswrap.h"

/* Subroutine */ int slasq3_(integer *i0, integer *n0, real *z__, integer *pp, 
	 real *dmin__, real *sigma, real *desig, real *qmax, integer *nfail, 
	integer *iter, integer *ndiv, logical *ieee, integer *ttype, real *
	dmin1, real *dmin2, real *dn, real *dn1, real *dn2, real *g, real *
	tau)
{
    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real s, t;
    integer j4, nn;
    real eps, tol;
    integer n0in, ipn4;
    real tol2, temp;
    extern /* Subroutine */ int slasq4_(integer *, integer *, real *, integer 
	    *, integer *, real *, real *, real *, real *, real *, real *, 
	    real *, integer *, real *), slasq5_(integer *, integer *, real *, 
	    integer *, real *, real *, real *, real *, real *, real *, real *, 
	     logical *), slasq6_(integer *, integer *, real *, integer *, 
	    real *, real *, real *, real *, real *, real *);
    extern doublereal slamch_(char *);
    extern logical sisnan_(real *);


/*  -- LAPACK routine (version 3.2)                                    -- */

/*  -- Contributed by Osni Marques of the Lawrence Berkeley National   -- */
/*  -- Laboratory and Beresford Parlett of the Univ. of California at  -- */
/*  -- Berkeley                                                        -- */
/*  -- November 2008                                                   -- */

/*  -- LAPACK is a software package provided by Univ. of Tennessee,    -- */
/*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..-- */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLASQ3 checks for deflation, computes a shift (TAU) and calls dqds. */
/*  In case of failure it changes shifts, and tries again until output */
/*  is positive. */

/*  Arguments */
/*  ========= */

/*  I0     (input) INTEGER */
/*         First index. */

/*  N0     (input) INTEGER */
/*         Last index. */

/*  Z      (input) REAL array, dimension ( 4*N ) */
/*         Z holds the qd array. */

/*  PP     (input/output) INTEGER */
/*         PP=0 for ping, PP=1 for pong. */
/*         PP=2 indicates that flipping was applied to the Z array */
/*         and that the initial tests for deflation should not be */
/*         performed. */

/*  DMIN   (output) REAL */
/*         Minimum value of d. */

/*  SIGMA  (output) REAL */
/*         Sum of shifts used in current segment. */

/*  DESIG  (input/output) REAL */
/*         Lower order part of SIGMA */

/*  QMAX   (input) REAL */
/*         Maximum value of q. */

/*  NFAIL  (output) INTEGER */
/*         Number of times shift was too big. */

/*  ITER   (output) INTEGER */
/*         Number of iterations. */

/*  NDIV   (output) INTEGER */
/*         Number of divisions. */

/*  IEEE   (input) LOGICAL */
/*         Flag for IEEE or non IEEE arithmetic (passed to SLASQ5). */

/*  TTYPE  (input/output) INTEGER */
/*         Shift type. */

/*  DMIN1, DMIN2, DN, DN1, DN2, G, TAU (input/output) REAL */
/*         These are passed as arguments in order to save their values */
/*         between calls to SLASQ3. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Function .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --z__;

    /* Function Body */
    n0in = *n0;
    eps = slamch_("Precision");
    tol = eps * 100.f;
/* Computing 2nd power */
    r__1 = tol;
    tol2 = r__1 * r__1;

/*     Check for deflation. */

L10:

    if (*n0 < *i0) {
	return 0;
    }
    if (*n0 == *i0) {
	goto L20;
    }
    nn = (*n0 << 2) + *pp;
    if (*n0 == *i0 + 1) {
	goto L40;
    }

/*     Check whether E(N0-1) is negligible, 1 eigenvalue. */

    if (z__[nn - 5] > tol2 * (*sigma + z__[nn - 3]) && z__[nn - (*pp << 1) - 
	    4] > tol2 * z__[nn - 7]) {
	goto L30;
    }

L20:

    z__[(*n0 << 2) - 3] = z__[(*n0 << 2) + *pp - 3] + *sigma;
    --(*n0);
    goto L10;

/*     Check  whether E(N0-2) is negligible, 2 eigenvalues. */

L30:

    if (z__[nn - 9] > tol2 * *sigma && z__[nn - (*pp << 1) - 8] > tol2 * z__[
	    nn - 11]) {
	goto L50;
    }

L40:

    if (z__[nn - 3] > z__[nn - 7]) {
	s = z__[nn - 3];
	z__[nn - 3] = z__[nn - 7];
	z__[nn - 7] = s;
    }
    if (z__[nn - 5] > z__[nn - 3] * tol2) {
	t = (z__[nn - 7] - z__[nn - 3] + z__[nn - 5]) * .5f;
	s = z__[nn - 3] * (z__[nn - 5] / t);
	if (s <= t) {
	    s = z__[nn - 3] * (z__[nn - 5] / (t * (sqrt(s / t + 1.f) + 1.f)));
	} else {
	    s = z__[nn - 3] * (z__[nn - 5] / (t + sqrt(t) * sqrt(t + s)));
	}
	t = z__[nn - 7] + (s + z__[nn - 5]);
	z__[nn - 3] *= z__[nn - 7] / t;
	z__[nn - 7] = t;
    }
    z__[(*n0 << 2) - 7] = z__[nn - 7] + *sigma;
    z__[(*n0 << 2) - 3] = z__[nn - 3] + *sigma;
    *n0 += -2;
    goto L10;

L50:
    if (*pp == 2) {
	*pp = 0;
    }

/*     Reverse the qd-array, if warranted. */

    if (*dmin__ <= 0.f || *n0 < n0in) {
	if (z__[(*i0 << 2) + *pp - 3] * 1.5f < z__[(*n0 << 2) + *pp - 3]) {
	    ipn4 = *i0 + *n0 << 2;
	    i__1 = *i0 + *n0 - 1 << 1;
	    for (j4 = *i0 << 2; j4 <= i__1; j4 += 4) {
		temp = z__[j4 - 3];
		z__[j4 - 3] = z__[ipn4 - j4 - 3];
		z__[ipn4 - j4 - 3] = temp;
		temp = z__[j4 - 2];
		z__[j4 - 2] = z__[ipn4 - j4 - 2];
		z__[ipn4 - j4 - 2] = temp;
		temp = z__[j4 - 1];
		z__[j4 - 1] = z__[ipn4 - j4 - 5];
		z__[ipn4 - j4 - 5] = temp;
		temp = z__[j4];
		z__[j4] = z__[ipn4 - j4 - 4];
		z__[ipn4 - j4 - 4] = temp;
/* L60: */
	    }
	    if (*n0 - *i0 <= 4) {
		z__[(*n0 << 2) + *pp - 1] = z__[(*i0 << 2) + *pp - 1];
		z__[(*n0 << 2) - *pp] = z__[(*i0 << 2) - *pp];
	    }
/* Computing MIN */
	    r__1 = *dmin2, r__2 = z__[(*n0 << 2) + *pp - 1];
	    *dmin2 = dmin(r__1,r__2);
/* Computing MIN */
	    r__1 = z__[(*n0 << 2) + *pp - 1], r__2 = z__[(*i0 << 2) + *pp - 1]
		    , r__1 = min(r__1,r__2), r__2 = z__[(*i0 << 2) + *pp + 3];
	    z__[(*n0 << 2) + *pp - 1] = dmin(r__1,r__2);
/* Computing MIN */
	    r__1 = z__[(*n0 << 2) - *pp], r__2 = z__[(*i0 << 2) - *pp], r__1 =
		     min(r__1,r__2), r__2 = z__[(*i0 << 2) - *pp + 4];
	    z__[(*n0 << 2) - *pp] = dmin(r__1,r__2);
/* Computing MAX */
	    r__1 = *qmax, r__2 = z__[(*i0 << 2) + *pp - 3], r__1 = max(r__1,
		    r__2), r__2 = z__[(*i0 << 2) + *pp + 1];
	    *qmax = dmax(r__1,r__2);
	    *dmin__ = -0.f;
	}
    }

/*     Choose a shift. */

    slasq4_(i0, n0, &z__[1], pp, &n0in, dmin__, dmin1, dmin2, dn, dn1, dn2, 
	    tau, ttype, g);

/*     Call dqds until DMIN > 0. */

L70:

    slasq5_(i0, n0, &z__[1], pp, tau, dmin__, dmin1, dmin2, dn, dn1, dn2, 
	    ieee);

    *ndiv += *n0 - *i0 + 2;
    ++(*iter);

/*     Check status. */

    if (*dmin__ >= 0.f && *dmin1 > 0.f) {

/*        Success. */

	goto L90;

    } else if (*dmin__ < 0.f && *dmin1 > 0.f && z__[(*n0 - 1 << 2) - *pp] < 
	    tol * (*sigma + *dn1) && dabs(*dn) < tol * *sigma) {

/*        Convergence hidden by negative DN. */

	z__[(*n0 - 1 << 2) - *pp + 2] = 0.f;
	*dmin__ = 0.f;
	goto L90;
    } else if (*dmin__ < 0.f) {

/*        TAU too big. Select new TAU and try again. */

	++(*nfail);
	if (*ttype < -22) {

/*           Failed twice. Play it safe. */

	    *tau = 0.f;
	} else if (*dmin1 > 0.f) {

/*           Late failure. Gives excellent shift. */

	    *tau = (*tau + *dmin__) * (1.f - eps * 2.f);
	    *ttype += -11;
	} else {

/*           Early failure. Divide by 4. */

	    *tau *= .25f;
	    *ttype += -12;
	}
	goto L70;
    } else if (sisnan_(dmin__)) {

/*        NaN. */

	if (*tau == 0.f) {
	    goto L80;
	} else {
	    *tau = 0.f;
	    goto L70;
	}
    } else {

/*        Possible underflow. Play it safe. */

	goto L80;
    }

/*     Risk of underflow. */

L80:
    slasq6_(i0, n0, &z__[1], pp, dmin__, dmin1, dmin2, dn, dn1, dn2);
    *ndiv += *n0 - *i0 + 2;
    ++(*iter);
    *tau = 0.f;

L90:
    if (*tau < *sigma) {
	*desig += *tau;
	t = *sigma + *desig;
	*desig -= t - *sigma;
    } else {
	t = *sigma + *tau;
	*desig = *sigma - (t - *tau) + *desig;
    }
    *sigma = t;

    return 0;

/*     End of SLASQ3 */

} /* slasq3_ */
