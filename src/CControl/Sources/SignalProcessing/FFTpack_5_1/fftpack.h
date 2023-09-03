#ifndef FFTPACK
#define FFTPACK

#include "f2c.h"
int rfft1i_(integer* n, real* wsave, integer* lensav, integer* ier);
int rfft1f_(integer* n, integer* inc, real* r__, integer* lenr, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int rfft1b_(integer* n, integer* inc, real* r__, integer* lenr, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int rfft2i_(integer* l, integer* m, real* wsave, integer* lensav, integer* ier);
int rfft2f_(integer* ldim, integer* l, integer* m, real* r__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int rfft2b_(integer* ldim, integer* l, integer* m, real* r__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);

#endif /* FFTPACK */
