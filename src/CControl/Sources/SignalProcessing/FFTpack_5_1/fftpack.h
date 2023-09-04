#ifndef FFTPACK
#define FFTPACK

#ifdef __cplusplus
extern "C" {
#endif

#include "f2c.h"

int cfft1i_(integer* n, real* wsave, integer* lensav, integer* ier);
int cfft1f_(integer* n, integer* inc, complex* c__, integer* lenc, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int cfft1b_(integer* n, integer* inc, complex* c__, integer* lenc, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);

int cfft2i_(integer* l, integer* m, real* wsave, integer* lensav, integer* ier);
int cfft2f_(integer* ldim, integer* l, integer* m, complex* c__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);
int cfft2b_(integer* ldim, integer* l, integer* m, complex* c__, real* wsave, integer* lensav, real* work, integer* lenwrk, integer* ier);

#ifdef __cplusplus
}
#endif

#endif /* FFTPACK */
