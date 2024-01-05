#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t cluster_filter(float X[], size_t row, size_t column, float epsilon, size_t min_pts);
void filtfilt(float y[], const float t[], const size_t l, const float K);
void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], size_t m, size_t p, size_t* k);
void sr_ukf_state_estimation(float y[], float xhat[], float Rn[], float Rv[], float u[], void (*F)(float[], float[], float[]), float S[], float alpha, float beta, size_t L);
void fft(float xr[], float xi[], size_t n);
void fft2(float XR[], float XI[], size_t row, size_t column);
void fftshift(float X[], size_t row, size_t column);
void ifft(float xr[], float xi[], size_t n);
void ifftshift(float X[], size_t row, size_t column);
void ifft2(float XR[], float XI[], size_t row, size_t column);

#ifdef __cplusplus
}
#endif

#endif /* !SIGNAL_PROCESSING_H_ */
