/*
 * Functions.h
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
#include <float.h>						/* Required for FLT_EPSILON */
#include <time.h> 						/* For srand, clock */

/* In ANSI C (C89), the __STDC_VERSION__ is not defined */
#ifndef __STDC_VERSION__
#define __STDC_VERSION__ 199409L		/* STDC version of C89 standard */
#endif

/* C99 has the __STDC_VERSION 199901L */
#if __STDC_VERSION__ < 199901L		
/* Math functions */
float sqrtf(float x);
float fabsf(float x);
float acosf(float x);
float expf(float x);
float powf(float base, float power);
float logf(float x);
float sinf(float x);

/* Standard signed int and unsigned int */
typedef unsigned char  uint8_t;
typedef signed char    int8_t;
typedef unsigned short uint16_t;
typedef signed short   int16_t;
typedef unsigned long  uint32_t;
typedef signed long    int32_t;

/* Standard bool */
typedef uint8_t bool;
#define true 1
#define false 0
#else
/* C99 and above */
#include <stdbool.h>					/* For bool datatype */
#include <stdint.h>						/* For uint8_t, uint16_t and uint16_t */
#endif

/* Define for all */
#define PI 2*acosf(0.0f)				/* acos(0) is pi/2 */

#ifdef __cplusplus
extern "C" {
#endif

/* Control engineering */
void lmpc(float A[], float B[], float C[], float x[], float u[], float r[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, bool has_integration);
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP);
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[], uint8_t RDIM);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], uint8_t ADIM, uint8_t NP, uint8_t NZ, bool integral_action);
bool stability(float A[], uint8_t ADIM);
void c2d(float A[], float B[], uint8_t ADIM, uint8_t RDIM, float sampleTime);

/* Miscellaneous */
void cat(uint8_t dim, float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b, uint16_t row_c, uint16_t column_c);
float saturation(float input, float lower_limit, float upper_limit);
void cut(float A[], uint16_t column, float B[], uint16_t start_row, uint16_t stop_row, uint16_t start_column, uint16_t stop_column);
void insert(float A[], float B[], uint16_t row_a, uint16_t column_a, uint16_t column_b, uint16_t startRow_b, uint16_t startColumn_b);
void print(float A[], uint16_t row, uint16_t column);
float sign(float number);
void randn(float x[], uint16_t length, float mu, float sigma);
float mean(float x[], uint16_t length);
float stddev(float x[], uint16_t length);
float vmax(float a, float b);
float vmin(float a, float b);
void sort(float X[], uint16_t row, uint16_t column, uint8_t dim, uint8_t mode);
void sum(float x[], float y[], uint16_t row, uint16_t column, uint8_t dim);

/* Filtering */
void filtfilt(float y[], float t[], uint16_t l, float K);
void pf(float x[], float xhat[], float xhatp[], float horizon[], float noise[], uint8_t m, uint8_t p, uint8_t *k);
void sr_ukf_state_estimation(float y[], float xhat[], float Rn[], float Rv[], float u[], void (*F)(float[], float[], float[]), float S[], float alpha, float beta, uint8_t L);

/* Linear algebra */
uint8_t inv(float* A, uint16_t row);
void linsolve_upper_triangular(float* A, float* x, float* b, uint16_t column);
void tran(float A[], uint16_t row, uint16_t column);
void mul(float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t column_b);
void svd_jacobi_one_sided(float A[], uint16_t row, float U[], float S[], float V[]);
void dlyap(float A[], float P[], float Q[], uint16_t row);
uint8_t svd_golub_reinsch(float A[], uint16_t row, uint16_t column, float U[], float S[], float V[]);
uint8_t qr(float A[], float Q[], float R[], uint16_t row_a, uint16_t column_a, bool only_compute_R);
void linsolve_qr(float A[], float x[], float b[], uint16_t row, uint16_t column);
void linsolve_lower_triangular(float A[], float x[], float b[], uint16_t row);
uint8_t lup(float A[], float LU[], uint8_t P[], uint16_t row);
float det(float A[], uint16_t row);
uint8_t linsolve_lup(float A[], float x[], float b[], uint16_t row);
void chol(float A[], float L[], uint16_t row);
void cholupdate(float L[], float x[], uint16_t row, bool rank_one_update);
void linsolve_chol(float A[], float x[], float b[], uint16_t row);
void pinv(float A[], uint16_t row, uint16_t column);
void hankel(float V[], float H[], uint16_t row_v, uint16_t column_v, uint16_t row_h, uint16_t column_h, uint16_t shift);
void balance(float A[], uint16_t row);
void eig(float A[], float wr[], float wi[], uint16_t row);
void eig_sym(float A[], uint16_t row, float d[]);
void eig_sym_generalized(float A[], float B[], uint16_t row, float d[]);
float norm(float A[], uint16_t row, uint16_t column, uint8_t l);
void expm(float A[], uint16_t row);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[], uint8_t elements, float alpha, float max_value, float min_value, bool random_guess_active);
void linsolve_gauss(float A[], float x[], float b[], uint16_t row, uint16_t column, float alpha);
  
/* Optimization */
bool linprog(float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a, uint8_t max_or_min);
bool quadprog(float Q[], float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a);

/* Statistics */
void pca(float X[], float W[], uint8_t components, uint16_t row, uint16_t column);
void lda(float X[], uint8_t y[], float W[], uint8_t components, uint16_t row, uint16_t column);

/* AI-algorithms */
void Astar(int map[], int path_x[], int path_y[], int x_start, int y_start, int x_stop, int y_stop, int height, int width, uint8_t norm_mode, int* steps);
uint8_t inpolygon(float x, float y, float px[], float py[], uint8_t p);

/* System identification */
void rls(uint8_t NP, uint8_t NZ, uint8_t NZE, float theta[], float u, float y, uint8_t* count, float* past_e, float* past_y, float* past_u, float phi[], float P[], float Pq, float forgetting);
void okid(float u[], float y[], float g[], uint16_t row, uint16_t column);
void era(float u[], float y[], uint16_t row, uint16_t column, float A[], float B[], float C[], uint8_t row_a, uint8_t inputs_outputs);
void sr_ukf_parameter_estimation(float d[], float what[], float Re[], float x[], void (*G)(float[], float[], float[]), float lambda_rls, float Sw[], float alpha, float beta, uint8_t L);

#ifdef __cplusplus
}
#endif

#endif /* CCONTROL_HEADERS_FUNCTIONS_H_ */
