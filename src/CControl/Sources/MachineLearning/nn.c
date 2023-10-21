/*
 * nn.c
 *
 *  Created on: 19 Augusti 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * This is a neural network. It's not a deep neural network.
 * If you want to use Deep Learning, just use multiple neural networks.
 * This neural network is special. Here I'm using Support Vector Machine
 * to train a neural network. The reason why is that Support Vector Machine
 * is using convex optimization a.k.a Quadratic Programming, and it gives a very good result
 * with small data. For example, with this technique, Ronald Fisher's Iris dataset, can give a
 * result of 96.7% accuracy with just one neural network with a nonlinear activation function.
 * Even if the Iris dataset (contains three classes), have two classes that are clustred together.
 * 
 * The neural network is:
 * y = weight*x + bias
 * 
 * Where the index of the maximum value of vector y, is the class ID for unknown data vector x
 * 
 * X[m*n]
 * class_id[m]
 * weight[(max(class_id) + 1)*n]
 * bias[(max(class_id) + 1)]
 * status[(max(class_id) + 1)]
 * accuracy[(max(class_id) + 1)]
 */
void nn(float X[], size_t class_id[], float weight[], float bias[], bool status[], float accuracy[], size_t row, size_t column, float C, float lambda) {
	/* Find the max classes */
	size_t classes = class_id[row - 1] + 1;

	/* Do SVM */
	float* labels = (float*)malloc(row * sizeof(float));
	size_t i, j;
	for (i = 0; i < classes; i++) {
		/* Tune in class ID */
		for (j = 0; j < row; j++) {
			/* Get the class ID */
			if (i == class_id[j]) {
				labels[j] = 1.0f;
			}
			else {
				labels[j] = -1.0f;
			}
		}
		/* Compute the model parameters w and bias b */
		status[i] = svm(X, labels, weight + column * i, &bias[i], &accuracy[i], C, lambda, row, column);

		/* Print the result */
		printf("Class %i: Success: %s. The accuracy = %f\n", i, status[i] ? "yes" : "no", accuracy[i]);
	}

	/* Free*/
	free(labels);
}