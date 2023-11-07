/*
 * layer_1.h
 *
 * Created on : 2023-11-07 20:41:07
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 19488
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

const static float layer_1_w[layer_1_row * layer_1_column] = { 6.501629017e-03f, 1.796848373e-03f, -4.266167816e-04f, 9.931317763e-04f, 7.088647690e-04f, 
7.088647690e-04f, -5.739516020e-02f, -4.656612873e-09f, 8.830014616e-03f, -1.766004786e-02f, 
-1.766004786e-02f, -3.532009572e-02f, -2.488234080e-03f, -3.388886340e-03f, 5.492887273e-02f, 
5.492887273e-02f, -1.045074780e-02f, 2.024535835e-02f, -2.763152122e-03f, -2.740090340e-02f };

const static float layer_1_b[layer_1_row] = { -1.383406401e+00f, 7.350996137e-01f, -1.209643245e+00f, 5.781204998e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
