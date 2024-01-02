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
#include "macros.h"

/* Memory leackage library for Visual Studio */
#ifdef _MSC_VER
#include <crtdbg.h>
#endif /* !_MSC_VER */

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

float ceilf(float x) {
	return (float)ceil(x);
}

float floorf(float x) {
	return (float)floor(x);
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
void centroid(const float X[], float C[], const size_t row, const size_t column);
float clusterratio(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
float clusterdistance(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
void dbscan(float X[], size_t idx[], float epsilon, size_t min_pts, size_t row, size_t column);
MODEL* fisherfaces(MODEL_SETTINGS* model_settings);
void kernel(const float X[], float K[], const size_t row, const size_t column, const float kernel_parameters[], const KERNEL_METHOD kernel_method);
bool kmeans(const float X[], size_t idx[], float C[], const size_t k, const size_t row, const size_t column);
void kpca(const float X[], float W[], float P[], const size_t c, const size_t row, const size_t column, const float kernel_parameters[], const KERNEL_METHOD kernel_method);
void kpca_lda_nn(MODEL_NN* model_nn, const MODEL_SETTINGS* model_settings);
uint32_t lbp(const float X[], const size_t row, const size_t column, const size_t x, const size_t y, const float init_angle, const float radius, LBP_BIT lbp_bit);
void nn_train(const float X[], const size_t class_id[], float weight[], float bias[], bool status[], float accuracy[], const size_t row, const size_t column, const size_t classes, const float C, const float lambda);
size_t nn_predict(const float model_w[], const float model_b[], const float x[], float y[], const size_t row_w, const size_t column_w, bool* class_id_found, const ACTIVATION_FUNCTION activation_function);
void nn_eval(const float model_w[], const float model_b[], const float X[], float Y[], const size_t class_id[], const size_t row_w, const size_t column_w, const size_t row_x, const ACTIVATION_FUNCTION activation_function);
void nn_save(const float model_w[], const float model_b[], const ACTIVATION_FUNCTION activation_function, const char model_path[], const char model_name[], const size_t row, const size_t column);
MODEL* ght(MODEL_SETTINGS* settings);
bool svm(float X[], float y[], float w[], float* b, float* accuracy, float C, float lambda, size_t row, size_t column);

/* Miscellaneous */
float anglevector(const float a[], const float b[], const size_t row);
void cat(const bool row_wise, const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const size_t column_b, const size_t row_c, const size_t column_c);
float saturation(const float input, const float lower_limit, const float upper_limit);
void cut(const float A[], size_t column, float B[], size_t start_row, size_t stop_row, size_t start_column, size_t stop_column);
size_t find(const float A[], int32_t index[], const float condition, const size_t row, const FIND_CONDITION_METOD condition_method);
void ind2sub(const size_t index, const size_t column, size_t* row_index, size_t* column_index);
void insert(const float A[], float B[], const size_t row_a, const size_t column_a, const size_t column_b, const size_t start_row_b, const size_t start_column_b);
bool issymmetric(const float A[], const size_t row, const size_t column);
float orientation(const float X[], const size_t radius, const size_t x, const size_t y, const size_t row, const size_t column);
void pdist2(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t row_b, const PDIST2_METRIC metric);
uint8_t popcount(const uint64_t x);
void print(const float A[], const size_t row, const size_t column);
float rad2deg(const float radians);
void rot2d(float x[], const float radians);
float deg2rad(const float radians);
void scalar(float x[], const float k, const size_t row);
float sign(const float number);
float vmax(const float a, const float b);
float vmin(const float a, const float b);
void sort(float X[], size_t index[], const size_t row, const size_t column, const SORT_MODE sort_mode);
void sum(float x[], float y[], size_t row, size_t column, bool row_direction);
void unit(float x[], const size_t row);

/* Hardware */
void concatenate_paths(char total_path[], const char path_first[], const char path_second[]);
size_t count_sub_folders(const char folder_path[]);
void detectmemoryleak();
size_t scan_file_names(const char folder_path[], char** file_names[], const char ending[]);
size_t scan_sub_folder_names(const char folder_path[], char** sub_folder_names[]);

/* Linear algebra */
bool inv(float A[], size_t row);
void linsolve_upper_triangular(float A[], float x[], float b[], size_t column);
void tran(float A[], const size_t row, const size_t column);
void mul(const float A[], const float B[], float C[], const size_t row_a, const size_t column_a, const size_t column_b);
size_t rank(float A[], size_t row, size_t column);
bool svd(float A[], size_t row, size_t column, float U[], float S[], float V[]);
void dlyap(float A[], float P[], float Q[], size_t row);
float dot(const float a[], const float b[], const size_t row);
bool qr(float A[], float Q[], float R[], size_t row_a, size_t column_a, bool only_compute_R);
bool linsolve_qr(float A[], float x[], float b[], size_t row, size_t column);
void linsolve_lower_triangular(float A[], float x[], float b[], size_t row);
bool lup(float A[], float LU[], int P[], size_t row);
float det(float A[], size_t row);
bool linsolve_lup(float A[], float x[], float b[], size_t row);
bool chol(float A[], float L[], size_t row);
void cholupdate(float L[], float x[], size_t row, bool rank_one_update);
void conv(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k, const CONV_SHAPE shape);
void conv2(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k, const size_t column_k, const CONV_SHAPE shape);
void conv2fft(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k);
void convfft(const float a[], const float k[], float b[], const size_t row_a, const size_t row_k);
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
void filtfilt(float y[], const float t[], const size_t l, const float K);
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
FAST_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);
FAST_XY* featuredetection(const uint8_t X[], int* N, const int fast_threshold, const FAST_METHOD fast_method, const int row, const int column);
float* fspecial(const float value, size_t* output_size, const FSPECIAL_TYPE type);
float* generalizedhough_create_data(const uint8_t image[], const size_t row, const size_t column, const int max_points, size_t* max_classes, const float sigma, const uint8_t fast_threshold, const FAST_METHOD fast_method, const float epsilon, const size_t min_pts, const GENERALIZED_HOUGH_METHOD generalized_hough_method);
GENERALIZED_HOUGH_MODEL* generalizedhough_create_model(const float X[], const size_t row);
void generalizedhough_eval_votes(const float X[], const GENERALIZED_HOUGH_MODEL model[], const float smoothing_accumulator, float* max_value_accumulator, size_t* xc, size_t* yc, const size_t row);
void generalizedhough_free_model(const GENERALIZED_HOUGH_MODEL model[]);
void harris(const float X[], float H[], const float sigma, const uint8_t edge, const size_t row, const size_t column);
size_t hough(const float X[], float* K[], float* M[], const float p, const float epsilon, const size_t min_pts, const size_t row, const size_t column);
MODEL* imcollect(const MODEL_SETTINGS* model_settings);
void imcollectfree(MODEL* model);
void imfree(PGM* image);
void pooling(float X[], float P[], size_t row, size_t column, size_t p, POOLING_METHOD pooling_method);
void rpca(float X[], float L[], float S[], size_t row, size_t column);
void sobel(const float X[], float G[], float O[], const size_t row, const size_t column, const SOBEL_METHOD sobel_method);

/* Statistics */
float amax(const float x[], size_t* max_index, const size_t length);
float amin(const float x[], size_t* min_index, const size_t length);
void center(float X[], float mu[], size_t row, size_t column);
float area(const float X[], const size_t row, size_t* total_elements, const AREA_METHOD area_method);
void randn(float x[], size_t length, float mu, float sigma);
void randperm(size_t x[], size_t N, size_t M);
float mean(float x[], size_t length);
float opa(const float X[], float Y[], const size_t row, const size_t column);
float var(float x[], size_t length);
float cov(float x[], float y[], size_t length);
void covm(float X[], float Y[], size_t row, size_t column);
float stddev(float x[], size_t length);
void pca(const float X[], float W[], float P[], float mu[], const size_t c, const size_t row, const size_t column);
void lda(const float X[], const size_t y[], float W[], float P[], const size_t c, const size_t row, const size_t column);

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
