#ifndef LINEAR_ALGEBRA_H_
#define LINEAR_ALGEBRA_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

bool inv(float A[], const size_t row);
void linsolve_upper_triangular(const float A[], float x[], const float b[], const size_t row);
void tran(float A[], const size_t row, const size_t column);
void mul(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t column_b);
size_t rank(float A[], size_t row, size_t column);
bool svd(const float A[], const size_t row, const size_t column, float U[], float S[], float V[]);
void dlyap(float A[], float P[], float Q[], size_t row);
float dot(const float a[], const float b[], const size_t row);
bool qr(float A[], float Q[], float R[], size_t row_a, size_t column_a, bool only_compute_R);
bool linsolve_qr(float A[], float x[], float b[], size_t row, size_t column);
void linsolve_lower_triangular(const float A[], float x[], const float b[], const size_t row);
bool lup(float A[], float LU[], int P[], size_t row);
float det(float A[], size_t row);
bool linsolve_lup(float A[], float x[], float b[], size_t row);
bool chol(const float A[], float L[], const size_t row);
void cholupdate(float L[], float x[], const size_t row, const bool rank_one_update);
void conv(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k, const CONV_SHAPE shape);
void conv2(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k, const size_t column_k, const CONV_SHAPE shape);
void conv2fft(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k);
void convfft(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k);
bool linsolve_chol(const float A[], float x[], const float b[], const size_t row);
void pinv(float A[], size_t row, size_t column);
bool hankel(float V[], float H[], size_t row_v, size_t column_v, size_t row_h, size_t column_h, size_t shift);
void balance(float A[], size_t row);
bool eig(const float A[], float dr[], float di[], float wr[], float wi[], size_t row);
bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]);
float norm(const float X[], const size_t row, const size_t column, const NORM_METHOD norm_method);
bool expm(float A[], const size_t row);
void eye(float I[], const float value, const size_t row, const size_t column);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[], size_t elements, float alpha, float max_value, float min_value, bool random_guess_active);
void linsolve_gauss(float A[], float x[], float b[], size_t row, size_t column, float alpha);

#ifdef __cplusplus
}
#endif

#endif /* !LINEAR_ALGEBRA_H_ */
