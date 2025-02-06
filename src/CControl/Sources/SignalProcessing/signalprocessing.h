#ifndef SIGNAL_PROCESSING_H_
#define SIGNAL_PROCESSING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t clusterfilter(float X[], const size_t row, const size_t column, const float epsilon, const uint8_t min_pts);
void filtfilt(float y[], const float t[], const size_t l, const float K);
void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], size_t m, size_t p, size_t* k);
void sr_ukf_state_estimation(const float y[], float xhat[], const float Rn[], const float Rv[], const float u[], void (*F)(float[], const float[], const float[]), float S[], const float alpha, const float beta, const size_t L);
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
