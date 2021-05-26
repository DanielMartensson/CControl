/*
 * Functions.h
 *
 *  Created on: 5 oct. 2019
 *      Author: Daniel Mårtensson
 */

#ifndef CCONTROL_FUNCTIONS_H_
#define CCONTROL_FUNCTIONS_H_

/*
 * Necessary libraries
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>	// required for FLT_EPSILON
#include <stdbool.h>
#define MAX_ITERATION_COUNT_SVD 30   // Maximum number of iterations for svd_jacobi_one_sided.c

/*
 * Control engineering
 */
void mpc(float* A, float* B, float* C, float* x, float* u, float* r, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t HORIZON, uint8_t ITERATION_LIMIT, bool has_integration);
void kalman(float* A, float* B, float* C, float* K, float* u, float* x, float* y, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM);
void lqi(float* y, float* u, float qi, float* r, float* L, float* Li, float* x, float* xi, uint8_t ADIM, uint8_t YDIM, uint8_t RDIM, uint8_t ANTI_WINDUP);
void mrac(float limit, float gain, float* y, float* u, float* r, float* I1, float* I2, uint8_t RDIM);
void theta2ss(float* A, float* B, float* C, float* theta, float* K, uint8_t ADIM, uint8_t NP, uint8_t NZ, uint8_t NZE, bool integral_action);
bool stability(float* A, uint8_t ADIM);
void c2d(float A[], float B[], uint8_t ADIM, uint8_t RDIM, float sampleTime);

/*
 * Miscellaneous
 */
float saturation(float input, float lower_limit, float upper_limit);
void cut(float* A, int row, int column, float* B, int start_row, int stop_row, int start_column, int stop_column);
void insert(float* A, float* B, int row_a, int column_a, int column_b, int startRow_b, int startColumn_b);
void print(float* A, int row, int column);
float sign(float number);
void filtfilt(float *y, float *t, uint16_t l, float K);

/*
 * Linear algebra
 */
uint8_t inv(float* A, int row);
void linsolve_upper_triangular(float* A, float* x, float* b, int column);
void tran(float* A, int row, int column);
void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b);
void svd_jacobi_one_sided(float *A, uint16_t row, uint8_t max_iterations, float *U, float *S, float *V);
void dlyap(float* A, float* P, float* Q, int row);
uint8_t svd_golub_reinsch(float *A, uint16_t row, uint16_t column, float *U, float *singular_values, float *V);
void qr(float* A, float* Q, float* R, int row_a, int column_a);
void triu(float* A, float* b, int row);
void linsolve_lower_triangular(float *A, float *x, float *b, int row);
void tikhonov(float* A, float* b, float* ATA, float* ATb, int row_a, int column_a, float alpha);
uint8_t lup(float *A, float *LU, int *P, int row);
float det(float *A, int row);
uint8_t linsolve_lup(float *A, float *x, float *b, int row);
void chol(float *A, float *L, int row);
void pinv(float* A, int row, int column);
void hankel(float* V, float* H, uint16_t row_v, uint16_t column_v, uint16_t row_h, uint16_t column_h, uint16_t shift);
void balance(float* A, uint16_t row);
void eig(float* A, float* wr, float* wi, uint16_t row);
void eig_sym(float* A, uint16_t row, float* d);
void sum(float A[], uint32_t row, uint32_t column, uint8_t l);
float norm(float A[], uint32_t row, uint32_t column, uint8_t l);
void expm(float A[], uint32_t row);
uint8_t linsolve_lsq(float* A, float* x, float* b, int row_a, int column_a, float alpha);

/*
 * Optimization
 */
void linprog(float* c, float* A, float* b, float* x, int row_a, int column_a, uint8_t max_or_min, int iteration_limit);

/*
 * AI-algorithms
 */
void Astar(int *map, int *path_x, int *path_y, int x_start, int y_start, int x_stop, int y_stop, int height, int width, int norm_mode, int* steps);

/*
 * System identification
 */
void rls(uint8_t NP, uint8_t NZ, uint8_t NZE, float* theta, float u, float y, uint8_t* count, float* past_e, float* past_y, float* past_u, float* phi, float* P, float Pq, float forgetting);
void okid(float* u, float* y, float* g, uint16_t row, uint16_t column);
void era(float* u, float* y, uint16_t row, uint16_t column, float* A, float* B, float* C, uint8_t row_a, uint8_t inputs_outputs);

#endif /* CCONTROL_FUNCTIONS_H_ */
