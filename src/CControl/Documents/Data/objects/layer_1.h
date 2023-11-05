/*
 * layer_1.h
 *
 * Created on : 2023-11-05 14:03:23
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 25806
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

const static float layer_1_w[layer_1_row * layer_1_column] = { 6.437499076e-03f, -4.395009018e-04f, 1.107749762e-03f, 6.457247073e-04f, 1.661332441e-03f, 
1.661332441e-03f, -6.117648631e-02f, 9.411763400e-03f, -1.882353425e-02f, -2.823530138e-02f, 
-2.823530138e-02f, -1.862645149e-09f, -1.089541242e-03f, 5.398699641e-02f, -1.282315888e-02f, 
-1.282315888e-02f, 1.746012457e-02f, -4.379585385e-03f, -2.078011632e-03f, -3.414702788e-02f };

const static float layer_1_b[layer_1_row] = { -1.377987385e+00f, 7.176473141e-01f, -1.187514186e+00f, 9.890192747e-02f };

const static ACTIVATION_FUNCTION layer_1_activtion_function = ACTIVTION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_1_H_ */
