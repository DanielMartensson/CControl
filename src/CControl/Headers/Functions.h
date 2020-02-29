/*
 * Functions.h
 *
 *  Created on: 5 oct. 2019
 *      Author: root
 */

#ifndef CCONTROL_FUNCTIONS_H_
#define CCONTROL_FUNCTIONS_H_

/*
 * Necessary libraries
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Configurations.h"

/*
 * Control functions
 */
void gpc(float* A, float* B, float* C, float* x, float* u, float* r);
void mpc(float* A, float* B, float* C, float* x, float* u, float* r);
void autotuning(float* A, float* B, float* C, float* L, float* Kr);
void kalman(float* A, float* B, float* C, float* K, float* u, float* x, float* y);
void lqi(float* y, float* u, float qi, float* r, float* L, float* Li, float* x, float* xi);
void mrac(float* y, float* u, float* r, float* K1, float* K2);

/*
 * Tools functions
 */
void theta2ss(float* A, float* B, float* C, float* theta, float* K);
void saturation(float* input, float lower_limit, float upper_limit);
void cut(float* A, int row, int column, float* B, int start_row, int stop_row, int start_column, int stop_column);
void insert(float* A, float* B, int row_a, int column_a, int column_b, int startRow_b, int startColumn_b);
void print(float* A, int row, int column);

/*
 * Algebra functions
 */
void inv(float* A, int row);
void linsolve(float* A, float* x, float* b, int row_a, int column_a);
void tran(float* A, int row, int column);
void mul(float* A, float* B, float* C, int row_a, int column_a, int column_b);
void linprog(float* c, float* A, float* b, float* x, int row_a, int column_a, int iteration_limit);
void svd(float *A, int row_a, float *U, float *S, float *V);
void dlyap(float* A, float* P, float* Q, int row_a);

/*
 * AI-algorithms
 */
void Astar(int *map, int *path_x, int *path_y, int x_start, int y_start, int x_stop, int y_stop, int height, int width, int norm_mode, int* steps);
/*
 * Machine learning algorithms
 */
void rls(float* theta, float u, float y, int* count, float* past_e, float* past_y, float* past_u, float* phi, float* P);

#endif /* CCONTROL_FUNCTIONS_H_ */
