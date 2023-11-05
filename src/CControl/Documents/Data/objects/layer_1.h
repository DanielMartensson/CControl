/*
 * layer_1.h
 *
 * Created on : 2023-11-05 14:43:41
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 30429
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

const static float layer_1_w[layer_1_row * layer_1_column] = { -5.195372505e-04f, 9.005470201e-04f, 6.684986874e-03f, 1.773477416e-03f, 2.346176188e-03f, 
2.346176188e-03f, 9.615387768e-03f, -1.442307886e-02f, -5.288461596e-02f, -4.807692021e-03f, 
-4.807692021e-03f, -4.326923564e-02f, 5.536822602e-02f, -7.981485687e-03f, -8.265200071e-03f, 
-8.265200071e-03f, 2.440950833e-03f, 1.646347344e-02f, -3.473560140e-02f, 1.516817417e-02f };

const static float layer_1_b[layer_1_row] = { -1.376831532e+00f, 7.692307830e-01f, -1.212611437e+00f, 5.252844095e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
