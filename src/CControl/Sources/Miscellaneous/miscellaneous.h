#ifndef MISCELLANIEOUS_H_
#define MISCELLANIEOUS_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

float anglevector(const float a[], const float b[], const size_t row);
void cat(const bool row_wise, const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const size_t column_b, const size_t row_c, const size_t column_c);
void cumsum(const float A[], float B[], const size_t row, const size_t column);
INLINE float saturation(const float input, const float lower_limit, const float upper_limit);
void cut(const float A[], size_t column, float B[], size_t start_row, size_t stop_row, size_t start_column, size_t stop_column);
size_t find(const float A[], int32_t index[], const float condition, const size_t row, const FIND_CONDITION_METOD condition_method);
uint8_t* float2uint(const float X[], const size_t row, const size_t column);
void ind2sub(const size_t index, const size_t column, size_t* row_index, size_t* column_index);
void insert(const float A[], float B[], const size_t row_a, const size_t column_a, const size_t column_b, const size_t start_row_b, const size_t start_column_b);
bool issymmetric(const float A[], const size_t row, const size_t column);
void normalize(float x[], const size_t row, const NORMALIZE_METHOD normalize_method);
void pdist2(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const PDIST2_METRIC metric);
INLINE uint8_t popcount(const uint32_t x);
void print(const float A[], const size_t row, const size_t column);
INLINE float rad2deg(const float radians);
float* radianvector(const size_t N);
void rot2d(float x[], const float radians);
INLINE float deg2rad(const float radians);
void scalar(float x[], const float k, const size_t row);
INLINE float sign(const float number);
INLINE float vmax(const float a, const float b);
INLINE float vmin(const float a, const float b);
void sort(float X[], size_t index[], const size_t row, const size_t column, const SORT_MODE sort_mode);
void sum(float x[], float y[], size_t row, size_t column, bool row_direction);
float* uint2float(const uint8_t X[], const size_t row, const size_t column);
void unit(float x[], const size_t row);

#ifdef __cplusplus
}
#endif

#endif /* !MISCELLANIEOUS_H_ */