#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

float amax(const float x[], size_t* max_index, const size_t length);
float amin(const float x[], size_t* min_index, const size_t length);
void center(float X[], float mu[], size_t row, size_t column);
float area(const float X[], const size_t row, size_t* total_elements, const AREA_METHOD area_method);
void randn(float x[], size_t length, float mu, float sigma);
void randperm(size_t x[], size_t N, size_t M);
float mean(float x[], size_t length);
INLINE float normpdf(const float x, const float mu, const float sigma);
float opa(const float X[], float Y[], const size_t row, const size_t column);
float var(float x[], size_t length);
float cov(float x[], float y[], size_t length);
void covm(float X[], float Y[], size_t row, size_t column);
INLINE float stddev(float x[], size_t length);
void pca(const float X[], float W[], float P[], float mu[], const size_t c, const size_t row, const size_t column);
void lda(const float X[], const uint8_t y[], float W[], float P[], const size_t c, const size_t row, const size_t column);

#ifdef __cplusplus
}
#endif

#endif // !STATISTICS_H_
