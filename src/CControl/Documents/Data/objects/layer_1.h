/*
 * layer_1.h
 *
 * Created on : 2023-11-05 16:36:29
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 30683
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
#define layer_1_column 5

const static float layer_1_w[layer_1_row * layer_1_column] = { 1.778990962e-03f, 2.140077995e-03f, 1.028672326e-03f, -5.015002680e-04f, 6.810702384e-03f, 
6.810702384e-03f, -4.728131928e-03f, -4.255319759e-02f, -1.891253144e-02f, 9.456269443e-03f, 
9.456269443e-03f, -5.200946331e-02f, 4.708780907e-03f, 1.807650551e-02f, -8.374558762e-03f, 
-8.374558762e-03f, 5.577334389e-02f, -8.713836782e-03f, -2.211838961e-02f, 3.435317427e-02f };

const static float layer_1_b[layer_1_row] = { -1.378579855e+00f, 7.730497718e-01f, -1.217827082e+00f, 2.522708476e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
