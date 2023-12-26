/*
 * conv2.c
 *
 *  Created on: 28 November 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * Convolutional matrix multiplication
 * A[row_a * column_a] - Input
 * K[row_k * column_k] - Kernel
 * 
 * B_same[row_a * column_a] - Output
 * B_valid[row_k * column_k] - Output
 * B_full[(row_a + row_k - 1) * (column_a + column_k - 1)] - Output
 * 
 * Found code at: https://blog.csdn.net/celerychen2009/article/details/38852105
 */
void conv2(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k, const size_t column_k, const CONV2_SHAPE shape) {
    /* Constants */
    const int src_row = row_a;
    const int src_cols = column_a;
    const int kernel_row = row_k;
    const int kernel_cols = column_k;

    /* Integer variables */
    int dst_row = 0;
    int dst_cols = 0;
    int edge_row = 0;
    int edge_cols = 0;
    int i, j, kernel_i, kernel_ii, kernel_j, kernel_jj, src_i, src_ii, src_j, src_jj;

    /* Select method */
    switch (shape) {
    case CONV2_SHAPE_FULL:
        dst_row = src_row + kernel_row - 1;
        dst_cols = src_cols + kernel_cols - 1;
        edge_row = kernel_row - 1;
        edge_cols = kernel_cols - 1;
        break;
    case CONV2_SHAPE_SAME:
        /* Check if kernel size is over CONV2_MAX_KERNEL_FFT_INSTEAD */
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
    case CONV2_SHAPE_SAME_NO_FFT:
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

    /* Pointer arrays */
    float* ptr_src_line_i;
    float* ptr_kernel_line_i;
    float* A0;
    float* K0;

    /* Variables */
    float sum;

    for (i = 0; i < dst_row; i++) {
        /* Boundary checks - Save the index */
        kernel_ii = kernel_row - 1.0f - vmax(0.0f, edge_row - i);
        src_ii = vmax(0.0f, i - edge_row);

        /* Iterate columns of the destination matrix */
        for (j = 0; j < dst_cols; j++) {
            /* Set sum to zero */
            sum = 0.0f;

            /* Reset the index */
            src_i = src_ii;
            kernel_i = kernel_ii;

            /* Jump */
            A0 = &A[src_i * src_cols];
            K0 = &K[kernel_i * kernel_cols];

            /* Boundary checks - Save the index */
            src_jj = vmax(0.0f, j - edge_cols);
            kernel_jj = kernel_cols - 1.0f - vmax(0.0f, edge_cols - j);

            /* Iterate the kernel rows */
            for (; kernel_i >= 0 && src_i < src_row; kernel_i--, src_i++) {
                /* Reset the index */
                src_j = src_jj;
                kernel_j = kernel_jj;

                /* Pointer to the source/kernel matrix pixel 
                ptr_src_line_i = &A[src_i * src_cols];
                ptr_kernel_line_i = &K[kernel_i * kernel_cols]; */
                ptr_src_line_i = A0;
                ptr_kernel_line_i = K0;

                /* Next column */
                ptr_src_line_i += src_j;
                ptr_kernel_line_i += kernel_j;

                /* Convolutional matrix multiplication */
                for (; kernel_j >= 0 && src_j < src_cols; kernel_j--, src_j++) {
                    sum += (*ptr_src_line_i++) * (*ptr_kernel_line_i--);
                }

                /* Next row */
                A0 += src_cols;
                K0 -= kernel_cols;
            }
            /* Add the sum */
            B[j] = sum;
        }

        /* Next row */
        B += dst_cols;
    }
}