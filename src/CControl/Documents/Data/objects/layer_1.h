/*
 * layer_1.h
 *
 * Created on : 2023-11-10 10:42:57
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 22886
 */

#ifndef layer_1_H_
#define layer_1_H_

/* Add the CControl header file */
#include "ccontrol.h"

/*
 * This is a Neural Network - A model that can classify raw data.
 * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.
 * This file contains one matrix called 'layer_1_w' and one vector called 'layer_1_b'.
 * To use this header file, your data that must be a vector of size 'layer_1_column' in row-major.
 * For example, if you have an image with size m * n, where m * n = 'layer_1_column', then you need to turn it into a vector in row-wise, not in column-wise.
 * Multiply your vector together with the matrix 'layer_1_w' and add vector 'layer_1_b'. Then you will get a new vector called y with the size 'layer_1_row'.
 * y = layer_1_w*your_data_vector + layer_1_b
 * The index of the largest number inside the vector y is the class ID of your_data_vector.
 * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.
 */

#define layer_1_row 4
#define layer_1_column 4

const static float layer_1_w[layer_1_row * layer_1_column] = { 2.365885675e-02f, -3.755737841e-02f, 4.190059006e-02f, -7.452640682e-02f, 
-1.484431326e-02f, -1.224883273e-02f, -5.445378646e-02f, -1.269723289e-02f, 
4.429288208e-02f, -2.044397593e-02f, 3.405645490e-03f, 6.473565847e-02f, 
-1.034014970e-01f, 1.331095099e-01f, 3.303021193e-02f, 1.844692230e-02f };

const static float layer_1_b[layer_1_row] = { -7.083738446e-01f, 2.934538603e+00f, -2.332067490e+00f, -4.824001312e+00f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
