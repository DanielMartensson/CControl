/*
 * layer_1.h
 *
 * Created on : 2023-11-05 13:01:01
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 11605
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

const static float layer_1_w[layer_1_row * layer_1_column] = { 1.123664202e-03f, 1.327790204e-03f, 6.767258514e-03f, 1.404206734e-03f, -3.841746366e-04f, 
-3.841746366e-04f, -1.017812081e-02f, -1.017812081e-02f, -5.597966537e-02f, -4.071248323e-02f, 
-4.071248323e-02f, 0.000000000e+00f, -4.631088115e-03f, 6.173633039e-03f, -6.635488942e-03f, 
-6.635488942e-03f, 1.347274706e-02f, 5.749323592e-02f, 2.612693235e-03f, -8.609198034e-03f };

const static float layer_1_b[layer_1_row] = { -1.340569735e+00f, 7.353692055e-01f, -1.246609926e+00f, 9.059254825e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
