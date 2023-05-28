/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel M�rtensson
 */

#include "../../Headers/Functions.h"

void fisherfaces_train(float X[], uint8_t y[], float W[], float P[], uint8_t components, uint16_t row, uint16_t column) {
	/* Compute PCA */
	float* Wpca = (float*)malloc(row * components * sizeof(float));
	float* Ppca = (float*)malloc(row * components * sizeof(float));
	pca(X, Wpca, Ppca, components, row, column);

	/* Compute LDA of Ppca */
	float* Wlda = (float*)malloc(row * components * sizeof(float));
	float* Plda = (float*)malloc(row * components * sizeof(float));
	lda(Ppca, y, Wlda, Plda, components, row, components);
	free(Plda);

	/* Compute W = Wpca*Wlda */
	mul(Wpca, Wlda, W, row, components, components);

	/* Compute P = W'*X - Notice that we keep W as transpose because we are going to use that later in prediction */
	/*tran(W, ? , ? );
	mul(W, X, P, ? , ? , column);*/
}

void fisherfaces_predict() {

}

