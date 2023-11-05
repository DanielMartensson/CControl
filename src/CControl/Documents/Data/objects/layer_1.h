/*
 * layer_1.h
 *
 * Created on : 2023-11-05 02:34:58
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 3744
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

const static float layer_1_w[layer_1_row * layer_1_column] = { -5.262942286e-04f, 1.796692959e-03f, 6.772483233e-03f, 2.017531544e-03f, 1.299373456e-03f, 
1.299373456e-03f, 9.501188993e-03f, -4.750592634e-03f, -4.750593007e-02f, -4.750593379e-02f, 
-4.750593379e-02f, -1.900237426e-02f, 5.506037548e-02f, 2.937481739e-03f, -6.419410929e-03f, 
-6.419410929e-03f, 1.601833478e-02f, -1.086127665e-02f, -3.550313413e-02f, -1.936307177e-02f };

const static float layer_1_b[layer_1_row] = { -1.369386196e+00f, 7.624701858e-01f, -1.228937745e+00f, -2.929259837e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
