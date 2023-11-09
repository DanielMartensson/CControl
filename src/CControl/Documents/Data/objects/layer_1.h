/*
 * layer_1.h
 *
 * Created on : 2023-11-09 17:09:27
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 27831
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

const static float layer_1_w[layer_1_row * layer_1_column] = { -4.257213324e-02f, -1.929119788e-02f, 1.060624793e-02f, 4.654415697e-02f, 
-1.793883368e-02f, -2.503215149e-02f, -1.343332604e-02f, -4.691827670e-02f, 
6.113479286e-02f, -4.367932677e-03f, 6.550223380e-02f, 1.746478677e-02f, 
5.107760429e-03f, 1.181051731e-01f, -1.170670092e-01f, 1.252001524e-02f };

const static float layer_1_b[layer_1_row] = { -2.282210112e+00f, 2.943168879e+00f, -4.178847790e+00f, -1.440275669e+00f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
