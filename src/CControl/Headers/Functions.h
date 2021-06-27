/*
 * Functions.h
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_HEADERS_FUNCTIONS_H_
#define CCONTROL_HEADERS_FUNCTIONS_H_

/* Necessary libraries */
#include <string.h>						// For memcpy, memset etc
#include <stdio.h>						// For printf
#include <stdlib.h>						// Standard library
#include <stdint.h>						// For uint8_t, uint16_t and uint16_t
#include <math.h>						// For sqrtf
#include <float.h>						// Required for FLT_EPSILON
#include <stdbool.h>					// For bool datatype
#include <time.h> 						// For srand, clock
#define MAX_ITERATION_COUNT_SVD 30   	// Maximum number of iterations for svd_jacobi_one_sided.c

/* Control engineering */
void mpc(float A[], float B[], float C[], float x[], float u[], float r[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, uint8_t ITERATION_LIMIT, bool has_integration);
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP);
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[], uint8_t RDIM);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], uint8_t ADIM, uint8_t NP, uint8_t NZ, uint8_t NZE, bool integral_action);
bool stability(float A[], uint8_t ADIM);
void c2d(float A[], float B[], uint8_t ADIM, uint8_t RDIM, float sampleTime);

/* Miscellaneous */
float saturation(float input, float lower_limit, float upper_limit);
void cut(float A[], uint16_t row, uint16_t column, float B[], uint16_t start_row, uint16_t stop_row, uint16_t start_column, uint16_t stop_column);
void insert(float A[], float B[], uint16_t row_a, uint16_t column_a, uint16_t column_b, uint16_t startRow_b, uint16_t startColumn_b);
void print(float A[], uint16_t row, uint16_t column);
float sign(float number);
void randn(float x[], uint16_t length, float mu, float sigma);
float mean(float x[], uint16_t length);
float std(float x[], uint16_t length);

/* Filtering */
void filtfilt(float y[], float t[], uint16_t l, float K);
void mcs_collect(float P[], uint16_t column_p, float x[], uint8_t row_x, float index_factor);
void mcs_estimate(float P[], uint16_t column_p, float x[], uint8_t row_x);
void mcs_clean(float P[], uint16_t column_p, uint8_t row_x);
void ukf(float xhat[], float zk[], float u[], float P[], float Q[], float R[], float a, float k, float b,  uint8_t L, void (*ukf_transition)(float[], float[], float[], uint8_t));

/* Linear algebra */
uint8_t inv(float* A, uint16_t row);
void linsolve_upper_triangular(float* A, float* x, float* b, uint16_t column);
void tran(float A[], uint16_t row, uint16_t column);
void mul(float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t column_b);
void svd_jacobi_one_sided(float A[], uint16_t row, uint8_t max_iterations, float U[], float S[], float V[]);
void dlyap(float A[], float P[], float Q[], uint16_t row);
uint8_t svd_golub_reinsch(float A[], uint16_t row, uint16_t column, float U[], float S[], float V[]);
void qr(float A[], float Q[], float R[], uint16_t row_a, uint16_t column_a);
void linsolve_qr(float A[], float x[], float b[], uint16_t row, uint16_t column);
void linsolve_lower_triangular(float A[], float x[], float b[], uint16_t row);
uint8_t lup(float A[], float LU[], uint8_t P[], uint16_t row);
float det(float A[], uint16_t row);
uint8_t linsolve_lup(float A[], float x[], float b[], uint16_t row);
void chol(float A[], float L[], uint16_t row);
void linsolve_chol(float A[], float x[], float b[], uint16_t row);
void pinv(float A[], uint16_t row, uint16_t column);
void hankel(float V[], float H[], uint16_t row_v, uint16_t column_v, uint16_t row_h, uint16_t column_h, uint16_t shift);
void balance(float A[], uint16_t row);
void eig(float A[], float wr[], float wi[], uint16_t row);
void eig_sym(float A[], uint16_t row, float d[]);
void sum(float A[], uint16_t row, uint16_t column, uint8_t l);
float norm(float A[], uint16_t row, uint16_t column, uint8_t l);
void expm(float A[], uint16_t row);
void nonlinsolve(void (*nonlinear_equation_system)(float[], float[], float[]), float b[], float x[], uint8_t elements, float alpha, float max_value, float min_value, bool random_guess_active);
void linsolve_gauss(float* A, float* x, float* b, int row, int column, float alpha);

/* Optimization */
void linprog(float c[], float A[], float b[], float x[], uint8_t row_a, uint8_t column_a, uint8_t max_or_min, uint8_t iteration_limit);

/* AI-algorithms */
void Astar(int map[], int path_x[], int path_y[], int x_start, int y_start, int x_stop, int y_stop, int height, int width, uint8_t norm_mode, int* steps);

/* System identification */
void rls(uint8_t NP, uint8_t NZ, uint8_t NZE, float theta[], float u, float y, uint8_t* count, float* past_e, float* past_y, float* past_u, float phi[], float P[], float Pq, float forgetting);
void okid(float u[], float y[], float g[], uint16_t row, uint16_t column);
void era(float u[], float y[], uint16_t row, uint16_t column, float A[], float B[], float C[], uint8_t row_a, uint8_t inputs_outputs);

#endif /* CCONTROL_HEADERS_FUNCTIONS_H_ */
