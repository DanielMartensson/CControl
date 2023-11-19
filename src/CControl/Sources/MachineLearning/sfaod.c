/*
 * sfaod.c
 *
 *  Created on: 28 oktober 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"


MODEL* sfaod(MODEL_SETTINGS* model_settings) {
	/* Header */
	printf("\t\t\t\tSobel FAST Angle Object Detection\n");

	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
	model_settings->settings_choice = SETTINGS_CHOICE_SFA;
	MODEL* model = imcollect(model_settings);

	/* Create model */
	kpca_lda_nn(&model->sfa_model, model_settings);

	/* Return model */
	return model;
}