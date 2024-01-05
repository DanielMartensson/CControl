/*
 * kernel.c
 *
 *  Created on: 27 Juli. 2023
 *      Author: Daniel Mårtensson
 */

#include "machinelearning.h"

 /*
  * This should be used with e.g before PCA or LDA for better classification/separation of classes-
  * This algorithm applies a kernel onto matrix K, e.g giving K another higher dimension
  * X[m*n]
  * K[m*m]
  * KERNEL_METHOD_LINEAR -> kernel_parameters can be NULL
  * KERNEL_METHOD_RBF -> kernel_parameters[gamma]
  * KERNEL_METHOD_POLY -> kernel_parameters[degree]
  * KERNEL_METHOD_SIGMOID -> kernel_parameters[alpha, beta]
  * KERNEL_METHOD_GAUSSIAN -> kernel_parameters[sigma]
  * KERNEL_METHOD_EXPONENTIAL -> kernel_parameters[sigma]
  */
void kernel(const float X[], float K[], const size_t row, const size_t column, const float kernel_parameters[], const KERNEL_METHOD kernel_method) {
	size_t i, j;
	switch (kernel_method) {
	case KERNEL_METHOD_LINEAR: {
		float* Xt = (float*)malloc(row * column * sizeof(float));
		memcpy(Xt, X, row * column * sizeof(float));
		tran(Xt, row, column);
		mul(X, Xt, K, row, column, row);
		free(Xt);
		break;
	}
	case KERNEL_METHOD_RBF: {
		pdist2(X, X, K, row, column, row, PDIST2_METRIC_EUCLIDEAN);
		const float gamma = kernel_parameters[0];
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = expf(-gamma * K[j] * K[j]);
			}
			K += row;
		}
		break;
	}
	case KERNEL_METHOD_POLY: {
		/* This will first compute K = X*X' */
		kernel(X, K, row, column, kernel_parameters, KERNEL_METHOD_LINEAR);
		/* Then we do K = K.^d */
		const float degree = kernel_parameters[0];
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = powf(K[j], degree);
			}
			K += row;
		}
		break;
	}
	case KERNEL_METHOD_SIGMOID:
		/* This will first compute K = X*X' */
		kernel(X, K, row, column, kernel_parameters, KERNEL_METHOD_LINEAR);
		/* Then we do K = tanh(alpha*K + beta) */
		const float alpha = kernel_parameters[0];
		const float beta = kernel_parameters[1];
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = tanhf(alpha * K[j] + beta);
			}
			K += row;
		}
		break;
	case KERNEL_METHOD_GAUSSIAN: {
		pdist2(X, X, K, row, column, row, PDIST2_METRIC_EUCLIDEAN);
		const float sigma = kernel_parameters[0];
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = expf(-K[j] * K[j] / (2 * sigma * sigma));
			}
			K += row;
		}
		break;
	}
	case KERNEL_METHOD_EXPONENTIAL: {
		pdist2(X, X, K, row, column, row, PDIST2_METRIC_EUCLIDEAN);
		const float sigma = kernel_parameters[0];
		for (i = 0; i < row; i++) {
			for (j = 0; j < row; j++) {
				K[j] = expf(-K[j] / (2 * sigma * sigma));
			}
			K += row;
		}
		break;
	}
	}
}

/* GNU Octave code:
*   
	function K = create_kernel(X, kernel_type, kernel_parameters)
	  % Select kernel type
	  switch kernel_type
		case 'gaussian'
		% Compute the euclidean distanses
		D = sqrt(distEucSq(X, X));
		sigma = kernel_parameters(1);
		K = exp(-D.^2 / (2 * sigma^2));

		case 'exponential'
		% Compute the euclidean distanses
		D = sqrt(distEucSq(X, X));
		sigma = kernel_parameters(1);
		K = exp(-D / (2 * sigma^2));

		case 'polynomial'
		degree = kernel_parameters(1);
		K = (X*X').^degree;

		case 'linear'
		K = X * X';

		case 'sigmoid'
		alpha = kernel_parameters(1);
		beta = kernel_parameters(2);
		K = tanh(alpha * X * X' + beta);

		case 'rbf'
		% Compute the euclidean distanses
		D = sqrt(distEucSq(X, X));
		gamma = kernel_parameters(1);
		K = exp(-gamma*D.^2);

		otherwise
		  error('Kernel type does not exist!');
		end
	end

	function D = distEucSq(X, Y)
	  Yt = Y';
	  XX = sum(X.*X,2);
	  YY = sum(Yt.*Yt,1);
	  D = abs(bsxfun(@plus,XX,YY)-2*X*Yt);
	end
*/