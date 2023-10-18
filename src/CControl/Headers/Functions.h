/*
 * functions.h
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_FUNCTIONS_H_
#define CCONTROL_HEADERS_FUNCTIONS_H_

/* C89 standard library */
#include <string.h>						/* For memcpy, memset etc */
#include <stdio.h>						/* For printf */
#include <stdlib.h>						/* Standard library */
#include <math.h>						/* For sqrtf */
#include <float.h>						/* Required for FLT_EPSILON, FLT_MAX, FLT_MIN */
#include <stddef.h>						/* Requried for NULL */
#include <time.h> 						/* For srand, clock */

/* CControl headers */
#include "defines.h"
#include "enums.h"
#include "structs.h"

#ifdef __cplusplus
extern "C" {
#endif

#if __STDC_VERSION___ < 199901L
#ifndef _MSC_VER
float sqrtf(float x) {
	return (float)sqrt(x);
}

float fabsf(float x) {
	return (float)fabs(x);
}

float acosf(float x) {
	return (float)acos(x);
}

float atan2f(float x) {
	return (float)atan(x);
}

float expf(float x) {
	return (float)exp(x);
}

float powf(float base, float power) {
	return (float)pow(base, power);
}

float logf(float x) {
	return (float)log(x);
}

float sinf(float x) {
	return (float)sin(x);
}

float tanhf(float x) {
	return (float)tanh(x);
}

float roundf(float x) {
	return (float)round(x);
}
#endif /* !_MSC_VER */
#endif /* !__STDC_VERSION___ */

/* Control engineering */
void lmpc(float A[], float B[], float C[], float x[], float u[], float r[], size_t ADIM, size_t YDIM, size_t RDIM, size_t HORIZON, bool has_integration);
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[], size_t ADIM, size_t YDIM, size_t RDIM);
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], size_t ADIM, size_t YDIM, size_t RDIM, ANTI_WINUP anti_winup);
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[], size_t RDIM);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], size_t ADIM, size_t NP, size_t NZ, bool integral_action);
bool stability(float A[], size_t ADIM);
void c2d(float A[], float B[], size_t ADIM, size_t RDIM, float sampleTime);

/* Machine learning */
void brisk(float X[], float sigma1, float sigma2, uint8_t threshold_sobel, int threshold_fast, FAST_METHOD fast_method, float histogram[], size_t row, size_t column);
void dbscan(float X[], size_t idx[], float epsilon, size_t min_pts, size_t row, size_t column);
FAST_XY* fast(const uint8_t X[], int row, int column, int threshold, int* num_corners, FAST_METHOD fast_method);
void fisherfaces(const char folder_path[], bool remove_outliers, const float espilon, const size_t min_pts, const size_t pooling_size, POOLING_METHOD pooling_method, const size_t components_pca, const float kernel_parameters[], KERNEL_METHOD kernel_method, const float C, const float lambda);
size_t hough(float X[], float* K[], float* M[], float p, float epsilon, size_t min_pts, size_t row, size_t column);
void kernel(float X[], float K[], size_t row, size_t column, float kernel_parameters[], KERNEL_METHOD kernel_method);
void kpca(float X[], float W[], float P[], size_t c, size_t row, size_t column, float kernel_parameters[], KERNEL_METHOD kernel_method);
uint32_t lbp(const float X[], const size_t row, const size_t column, const size_t x, const size_t y, const float init_angle, const uint8_t radius, LBP_BIT lbp_bit);
void nn(float X[], size_t class_id[], float weight[], float bias[], bool status[], float accuracy[], size_t row, size_t column, float C, float lambda);
bool svm(float X[], float y[], float w[], float* b, float* accuracy, float C, float lambda, size_t row, size_t column);

/* Miscellaneous */
void cat(bool row_wise, float A[], float B[], float C[], size_t row_a, size_t column_a, size_t row_b, size_t column_b, size_t row_c, size_t column_c);
float saturation(float input, float lower_limit, float upper_limit);
void cut(float A[], size_t column, float B[], size_t start_row, size_t stop_row, size_t start_column, size_t stop_column);
void find(float A[], int32_t index[], float condition, size_t row, FIND_CONDITION_METOD condition_method);
void insert(float A[], float B[], size_t row_a, size_t column_a, size_t column_b, size_t startRow_b, size_t startColumn_b);
bool issymmetric(float A[], size_t row, size_t column);
void pdist2(float A[], float B[], float C[], size_t row_a, size_t column_a, size_t row_b, PDIST2_METRIC metric);
void print(float A[], size_t row, size_t column);
float rad2deg(float radians);
float deg2rad(float radians);
float sign(float number);
float vmax(float a, float b);
float vmin(float a, float b);
void sort(float X[], size_t index[], size_t row, size_t column, SORT_MODE sort_mode);
void sum(float x[], float y[], size_t row, size_t column, bool row_direction);

/* Hardware */
void concatenate_paths(char total_path[], const char path_first[], const char path_second[]);
size_t count_sub_folders(const char folder_path[]);
size_t scan_file_names(const char folder_path[], char** file_names[]);
size_t scan_sub_folder_names(const char folder_path[], char** sub_folder_names[]);

/* Linear algebra */
bool inv(float A[], size_t row);
void linsolve_upper_triangular(float A[], float x[], float b[], size_t column);
void tran(float A[], size_t row, size_t column);
void mul(float A[], float B[], float C[], size_t row_a, size_t column_a, size_t column_b);
size_t rank(float A[], size_t row, size_t column);
bool svd(float A[], size_t row, size_t column, float U[], float S[], float V[]);
void dlyap(float A[], float P[], float Q[], size_t row);
float dot(float a[], float b[], size_t row);
bool qr(float A[], float Q[], float R[], size_t row_a, size_t column_a, bool only_compute_R);
bool linsolve_qr(float A[], float x[], float b[], size_t row, size_t column);
void linsolve_lower_triangular(float A[], float x[], float b[], size_t row);
bool lup(float A[], float LU[], int P[], size_t row);
float det(float A[], size_t row);
bool linsolve_lup(float A[], float x[], float b[], size_t row);
bool chol(float A[], float L[], size_t row);
void cholupdate(float L[], float x[], size_t row, bool rank_one_update);
void conv2fft(float A[], float B[], size_t row_a, size_t column_a, float K[], size_t row_k);
bool linsolve_chol(float A[], float x[], float b[], size_t row);
void pinv(float A[], size_t row, size_t column);
bool hankel(float V[], float H[], size_t row_v, size_t column_v, size_t row_h, size_t column_h, size_t shift);
void balance(float A[], size_t row);
bool eig(float A[], float dr[], float di[], float wr[], float wi[], size_t row);
bool eig_generalized(float A[], float B[], size_t row, float dr[], float di[], float wr[], float wi[]);
float norm(float X[], size_t row, size_t column, NORM_METHOD norm_method);
bool expm(float A[], size_t row);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[], size_t elements, float alpha, float max_value, float min_value, bool random_guess_active);
void linsolve_gauss(float A[], float x[], float b[], size_t row, size_t column, float alpha);
  
/* Optimization */
bool linprog(float c[], float A[], float b[], float x[], size_t row_a, size_t column_a, bool maximization);
bool lsqnonneg(float A[], float x[], float b[], float residual[], size_t row, size_t column);
bool quadprog(float Q[], float c[], float A[], float b[], float G[], float h[], float x[], size_t row_a, size_t row_g, size_t column_a, bool equality_constraints_are_used);

/* Signal processing */
size_t cluster_filter(float X[], size_t row, size_t column, float epsilon, size_t min_pts);
void filtfilt(float y[], float t[], size_t l, float K);
void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], size_t m, size_t p, size_t* k);
void sr_ukf_state_estimation(float y[], float xhat[], float Rn[], float Rv[], float u[], void (*F)(float[], float[], float[]), float S[], float alpha, float beta, size_t L);
void fft(float xr[], float xi[], size_t n);
void fft2(float XR[], float XI[], size_t row, size_t column);
void fftshift(float X[], size_t row, size_t column);
void ifft(float xr[], float xi[], size_t n);
void ifftshift(float X[], size_t row, size_t column);
void ifft2(float XR[], float XI[], size_t row, size_t column);

/* Image processing */
void imshow(PGM* image);
PGM* imread(const char file_path[]);
void imresize(float X[], float Y[], float d, size_t row, size_t column);
void imgaussfilt(float X[], float sigma, size_t row, size_t column);
void imfree(PGM* image);
void pooling(float A[], float P[], size_t row_a, size_t column_a, size_t p, POOLING_METHOD pooling_method);
void rpca(float X[], float L[], float S[], size_t row, size_t column);
void sobel(float X[], float G[], float O[], size_t row, size_t column, bool only_compute_G);

/* Statistics */
float amax(float x[], size_t* max_index, size_t length);
void center(float X[], float mu[], size_t row, size_t column);
void randn(float x[], size_t length, float mu, float sigma);
void randperm(size_t A[], size_t N, size_t M);
float mean(float x[], size_t length);
float var(float x[], size_t length);
float cov(float x[], float y[], size_t length);
void covm(float X[], float Y[], size_t row, size_t column);
float stddev(float x[], size_t length);
void pca(float X[], float W[], float P[], float mu[], size_t c, size_t row, size_t column);
void lda(float X[], size_t y[], float W[], float P[], size_t c, size_t row, size_t column);

/* AI-algorithms */
void Astar(int32_t map[], int32_t path_x[], int32_t path_y[], size_t x_start, size_t y_start, size_t x_stop, size_t y_stop, size_t height, size_t width, ASTAR_MODE astar_mode, size_t* steps);
bool inpolygon(float x, float y, float px[], float py[], size_t p);

/* System identification */
void rls(size_t NP, size_t NZ, size_t NZE, float theta[], float u, float y, size_t* count, float* past_e, float* past_y, float* past_u, float phi[], float P[], float Pq, float forgetting);
void okid(float u[], float y[], float g[], size_t row, size_t column);
void era(float u[], float y[], size_t row, size_t column, float A[], float B[], float C[], size_t row_a, size_t inputs_outputs);
void sr_ukf_parameter_estimation(float d[], float what[], float Re[], float x[], void (*G)(float[], float[], float[]), float lambda_rls, float Sw[], float alpha, float beta, size_t L);

#ifdef __cplusplus
}
#endif

#endif /* !CCONTROL_HEADERS_FUNCTIONS_H_ */
