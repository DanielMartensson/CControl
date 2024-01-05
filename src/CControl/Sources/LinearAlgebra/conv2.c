/*
 * conv2.c
 *
 *  Created on: 4 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "linearalgebra.h"

 /*
  * Convolutional matrix multiplication
  * A[row_a * column_a] - Input
  * K[row_k * column_k] - Kernel
  *
  * B_same[row_a * column_a] - Output
  * B_valid[(row_a - row_k + 1) * (column_a - column_k + 1)] - Output
  * B_full[(row_a + row_k - 1) * (column_a + column_k - 1)] - Output
  *
  * When using CONV_SHAPE_SAME_SEPARABLE_KERNEL
  * Then K[row_k] as a kernel vector
  *
  */
void conv2(const float A[], const float K[], float B[], const size_t row_a, const size_t column_a, const size_t row_k, const size_t column_k, const CONV_SHAPE shape) {
    /* Variables */
    size_t i, j, k, l;

    /* Select 2D convolution */
    switch (shape) {
    case CONV_SHAPE_FULL: {
        const size_t padding = row_k - 1;
        const size_t row_a1 = row_a + 2 * padding;
        const size_t column_a1 = column_a + 2 * padding;
        const size_t row_b = row_a + row_k - 1;
        const size_t column_b = column_a + column_k - 1;
        memset(B, 0, row_b * column_b * sizeof(float));
        float* A1 = (float*)malloc(row_a1 * column_a1 * sizeof(float));
        memset(A1, 0, row_a1 * column_a1 * sizeof(float));
        insert(A, A1, row_a, column_a, column_a1, padding, padding);
        for (i = 0; i < row_b; i++) {
            for (j = 0; j < column_b; j++) {
                for (k = 0; k < row_k; k++) {
                    for (l = 0; l < column_k; l++) {
                        B[j] += A1[(i + k) * column_a1 + j + l] * K[k * column_k + l];
                    }
                }
            }

            /* New row for B */
            B += column_b;
        }

        /* Free */
        free(A1);

        break;
    }
    case CONV_SHAPE_SAME: {
        /* Check if kernel size is over CONV_MAX_KERNEL_FFT_INSTEAD */
        if (row_k > CONV_MAX_KERNEL_FFT_INSTEAD) {
            /* row_k must be an odd number */
            if (row_k % 2 >= 1) {
                /* Do FFT instead */
                conv2fft(A, K, B, row_a, column_a, row_k);
                return;
            }
        }
        conv2(A, K, B, row_a, column_a, row_k, column_k, CONV_SHAPE_SAME_NO_FFT);
        break;
    }case CONV_SHAPE_SAME_NO_FFT: {
        const size_t padding = (row_k - 1) / 2;
        const size_t row_a1 = row_a + 2 * padding;
        const size_t column_a1 = column_a + 2 * padding;
        memset(B, 0, row_a * column_a * sizeof(float));
        float* A1 = (float*)malloc(row_a1 * column_a1 * sizeof(float));
        memset(A1, 0, row_a1 * column_a1 * sizeof(float));
        insert(A, A1, row_a, column_a, column_a1, padding, padding);
        for (i = 0; i < row_a; i++) {
            for (j = 0; j < column_a; j++) {
                for (k = 0; k < row_k; k++) {
                    for (l = 0; l < column_k; l++) {
                        B[j] += A1[(i + k) * column_a1 + j + l] * K[k * column_k + l];
                    }
                }
            }

            /* New row for B */
            B += column_a;
        }

        /* Free */
        free(A1);
        break;
    }case CONV_SHAPE_SAME_SEPARABLE_KERNEL: {
        float* B_copy = (float*)malloc(row_a * column_a * sizeof(float));
        float* B_copy0 = B_copy;
        float* B0 = B;
        for (i = 0; i < row_a; i++) {
            conv(A, K, B_copy, column_a, row_k, CONV_SHAPE_SAME);

            /* Next row */
            A += column_a;
            B_copy += column_a;
        }

        /* Reset */
        B_copy = B_copy0;

        /* Transpose of B copy */
        tran(B_copy, row_a, column_a);

        /* Again */
        for (i = 0; i < column_a; i++) {
            conv(B_copy, K, B, row_a, row_k, CONV_SHAPE_SAME);

            /* Next row */
            B_copy += row_a;
            B += row_a;
        }

        /* Reset */
        B_copy = B_copy0;
        B = B0;

        /* Free */
        free(B_copy);

        /* Transpose of B */
        tran(B, column_a, row_a);

        return;
    }case CONV_SHAPE_VALID: {
        const size_t row_b = row_a - row_k + 1;
        const size_t column_b = column_a - column_k + 1;
        memset(B, 0, row_b * column_b * sizeof(float));
        for (i = 0; i < row_b; i++) {
            for (j = 0; j < column_b; j++) {
                for (k = 0; k < row_k; k++) {
                    for (l = 0; l < column_k; l++) {
                        B[j] += A[(i + k) * column_a + j + l] * K[k * column_k + l];
                    }
                }
            }

            /* New row for B */
            B += column_b;
        }

        break;
    }
    default:
        printf("No support for that shape yet\n");
        return;
    }
}

/* This is a special case were you can use 1D convolution for separable kernels
                A = [-1.050452,   0.414110,   0.149972,   0.994917, -0.209254,   0.589792,   0.584076, -0.083662,   0.177192, 0.246984,
                    1.509006,  -1.333137, -1.660069,   0.681378, -1.053084, -1.337782,   0.225150, -0.268287,   0.832647, -0.104689,
                    0.109541,   0.434334,   0.268967, -1.115649, -1.887634,   1.789973, -0.129688, -0.426573,   0.265466, -1.561944,
                  0.081410,   0.803230, -0.014688,   0.350058, -0.246994,   1.392739, -0.210225, -0.348374,   0.280731, -1.255628,
                 -1.760850,   0.195185,   1.921482, -1.712214, -1.220659, -1.123662, -0.583558,   0.504691, -0.939355, 0.589897,
                 -0.971111,   0.685378, -1.280834,   0.718914, -0.988931,   0.805904,   0.540353, -0.224010,   1.270656, 0.569799,
                 -0.357689,  -1.730907,   2.075645, -1.206756, -0.552441, -0.123188, -0.015025, -2.148288, -0.647478, -1.287194,
                    0.500089,   0.027562,   0.015011, -0.981528,   0.634455, -1.214154,   0.340079, -1.651628,   1.055575, -0.774350,
                    1.292942,   0.051219, -0.591766, -0.375308, -3.435261, -0.482075, -1.488143, -1.242304,   0.021370, 1.358115,
                    0.278103,   3.624124, -0.263088,   0.545802,   0.271732,   0.225901,   0.219443,   0.025991, -1.009268, 0.127537];

        sigma = 0.8;
        kernel_size = round(6 * sigma);
        [x, y] = meshgrid(-kernel_size:kernel_size, -kernel_size:kernel_size);
        K = 1/(2*pi*sigma^2)*exp(-(x.^2 + y.^2)/(2*sigma^2));
        result = conv2(A, K, 'same');

        x = -kernel_size:kernel_size;
        y = x.';
        a = 1/(sqrt(2*pi)*sigma);
        b = 2*sigma*sigma;
        kr = 1/(sqrt(2*pi)*sigma) * exp(-x.^2/b);
        kc = 1/(sqrt(2*pi)*sigma) * exp(-y.^2/(2*sigma^2));

        result_sep = NaN(size(A));
        for r = 1:size(A,1)
            result_sep(r,:) = conv(A(r,:), kr, 'same');
        end
        result_sep = result_sep';
        for c = 1:size(A,2)
            result_sep(c, :) = conv(result_sep(c, :), kr, 'same');
        end
        result_sep = result_sep'
        max(max(abs(result-result_sep)))
*/