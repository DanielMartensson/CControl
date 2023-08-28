#ifndef FFTPACK
#define FFTPACK

void __ogg_fdrffti(int n, float* wsave, int* ifac);				/* initialize rfftf and rfftb */
void __ogg_fdrfftf(int n, float* r, float* wsave, int* ifac);	/* forward transform of a real periodic sequence */
void __ogg_fdrfftb(int n, float* r, float* wsave, int* ifac);	/* backward transform of a real coefficient array */

void __ogg_fdcosqi(int n, float* wsave, int* ifac);				/* initialize cosqf and cosqb */
void __ogg_fdcosqf(int n, float* x, float* wsave, int* ifac);	/* forward cosine transform with odd wave numbers */
void __ogg_fdcosqb(int n, float* x, float* wsave, int* ifac);	/* unnormalized inverse of cosqf */

#endif /* !FFTPACK */

