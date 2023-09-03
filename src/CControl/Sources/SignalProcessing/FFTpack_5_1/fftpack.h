#ifndef FFTPACK
#define FFTPACK

#ifdef __cplusplus
extern "C" {
#endif

#include "f2c.h"
int rfft2i_(integer* l, integer* m, real* wsave, integer* lensav, integer* ier);
int rfft2f_(integer* ldim, integer* l, integer* m, real* r__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int rfft2b_(integer* ldim, integer* l, integer* m, real* r__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);

#ifdef __cplusplus
}
#endif

#endif /* FFTPACK */
