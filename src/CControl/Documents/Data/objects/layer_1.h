/*
 * layer_1.h
 *
 * Created on : 2023-11-05 20:28:18
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 9884
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

const static float layer_1_w[layer_1_row * layer_1_column] = { 6.827206351e-03f, 2.087718341e-03f, 1.889866195e-03f, 1.241361140e-03f, -4.242284922e-04f, 
-4.242284922e-04f, -4.784689471e-02f, -1.435406692e-02f, -4.784689099e-02f, -1.435406785e-02f, 
-1.435406785e-02f, 0.000000000e+00f, -4.418019205e-03f, -6.725821644e-03f, 1.288278215e-02f, 
1.288278215e-02f, -4.238428548e-03f, 5.799446627e-02f, -1.635679603e-02f, -1.410987228e-03f };

const static float layer_1_b[layer_1_row] = { -1.349741697e+00f, 7.416268587e-01f, -1.249687433e+00f, 6.992325187e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
