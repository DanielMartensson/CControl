#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

float amax(const float x[], size_t* max_index, const size_t length);
float amin(const float x[], size_t* min_index, const size_t length);
void center(float X[], float mu[], size_t row, size_t column);
void centroid(const float X[], float C[], const size_t row, const size_t column);
float clusterratio(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
float clusterdistance(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
float area(const float X[], const size_t row, size_t* total_elements, const AREA_METHOD area_method);
size_t randi(const size_t x);
void randn(float x[], size_t length, float mu, float sigma);
void randperm(size_t x[], size_t N, size_t M);
float mean(const float x[], const size_t length);
float normpdf(const float x, const float mu, const float sigma);
float opa(const float X[], float Y[], const size_t row, const size_t column);
float var(const float x[], const size_t length);
float cov(const float x[], const float y[], const size_t length);
void covm(float X[], float Y[], size_t row, size_t column);
float giniimpurity(float x[], const bool is_numerical, const bool labels[], float* threshold, const size_t row);
float stddev(const float x[], const size_t length);
void pca(const float X[], float W[], float P[], float mu[], const size_t c, const size_t row, const size_t column);
void lda(const float X[], const uint8_t y[], float W[], float P[], const size_t c, const size_t row, const size_t column);

#ifdef __cplusplus
}
#endif

#endif // !STATISTICS_H_
