/*
 * conv2.c
 *
 *  Created on: 28 November 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Convolutional matrix multiplication
 * A[m*n] - Input
 * K[row_k*column_k] - Kernel
 * 
 * Output for "same":
 * B[row_a*column_a]
 * 
 * Found code at: https://blog.csdn.net/celerychen2009/article/details/38852105
 */
void conv2(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k, const size_t column_k, const CONV2_SHAPE shape) {
    /* Cleare sizes */
    int src_row = row_a;
    int src_cols = column_a;
    int kernel_row = row_k;
    int kernel_cols = column_k;
    int dst_row = 0;
    int dst_cols = 0;
    int edge_row = 0;
    int edge_cols = 0;
    int i, j, kernel_i, kernel_j, src_i, src_j;

    switch (shape) {
    case CONV2_SHAPE_FULL:
        dst_row = src_row + kernel_row - 1;
        dst_cols = src_cols + kernel_cols - 1;
        edge_row = kernel_row - 1;
        edge_cols = kernel_cols - 1;
        break;
    case CONV2_SHAPE_SAME:
        /* Check if kernel size is over 11 */
        if (row_k > CONV2_MAX_KERNEL_FFT_INSTEAD) {
            /* row_k must be an odd number */
            if (row_k % 2 >= 1) {
                /* Do FFT instead */
                conv2fft(A, B, row_a, column_a, K, row_k);
                return;
            }
        }

        dst_row = src_row;
        dst_cols = src_cols;
        edge_row = (kernel_row - 1) / 2;
        edge_cols = (kernel_cols - 1) / 2;
        break;
    case CONV2_SHAPE_VALID:
        dst_row = src_row - kernel_row + 1;
        dst_cols = src_cols - kernel_cols + 1;
        edge_row = edge_cols = 0;
        break;
    }

    float* ptr_dst_mat = B;
    float* src_mat = A;
    float* kernel_mat = K;

    for (i = 0; i < dst_row; i++) {
        float* ptr_dst_line_i = &ptr_dst_mat[i * dst_cols];
        for (j = 0; j < dst_cols; j++) {
            float sum = 0.0f;

            kernel_i = kernel_row - 1 - vmax(0, edge_row - i);
            src_i = vmax(0, i - edge_row);

            for (; kernel_i >= 0 && src_i < src_row; kernel_i--, src_i++) {
                float* ptr_src_line_i, * ptr_kernel_line_i;

                kernel_j = kernel_cols - 1 - vmax(0, edge_cols - j);
                src_j = vmax(0, j - edge_cols);

                ptr_src_line_i = &src_mat[src_i * src_cols];
                ptr_kernel_line_i = &kernel_mat[kernel_i * kernel_cols];

                ptr_src_line_i += src_j;
                ptr_kernel_line_i += kernel_j;

                for (; kernel_j >= 0 && src_j < src_cols; kernel_j--, src_j++) {
                    sum += *ptr_src_line_i++ * *ptr_kernel_line_i--;
                }
            }
            ptr_dst_line_i[j] = sum;
        }
    }
}