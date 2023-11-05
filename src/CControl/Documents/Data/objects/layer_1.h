/*
 * layer_1.h
 *
 * Created on : 2023-11-05 21:33:10
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 30713
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

const static float layer_1_w[layer_1_row * layer_1_column] = { 1.707877265e-03f, 6.980174687e-03f, -4.429370165e-04f, 1.914295717e-03f, 1.796648256e-03f, 
1.796648256e-03f, -4.199476168e-02f, -5.249345675e-02f, 0.000000000e+00f, -5.249345675e-03f, 
-5.249345675e-03f, -2.099738084e-02f, 1.515918225e-02f, -6.495369598e-03f, 5.655500293e-02f, 
5.655500293e-02f, 4.175745882e-03f, -8.627397940e-03f, 3.480134159e-02f, -8.840829134e-03f };

const static float layer_1_b[layer_1_row] = { -1.364115953e+00f, 7.375330329e-01f, -1.227191210e+00f, 7.049748302e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
