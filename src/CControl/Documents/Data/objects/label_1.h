/*
 * label_1.h
 *
 * Created on : 2023-11-05 20:54:49
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 5702
 */

#ifndef label_1_H_
#define label_1_H_

/* Add the CControl header file */
#include "ccontrol.h"

/*
 * This is a Neural Network - A model that can classify raw data.
 * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.
 * This file contains one matrix called 'label_1_w' and one vector called 'label_1_b'.
 * To use this header file, your data that must be a vector of size 'label_1_column' in row-major.
 * For example, if you have an image with size m * n, where m * n = 'label_1_column', then you need to turn it into a vector in row-wise, not in column-wise.
 * Multiply your vector together with the matrix 'label_1_w' and add vector 'label_1_b'. Then you will get a new vector called y with the size 'label_1_row'.
 * y = label_1_w*your_data_vector + label_1_b
 * The index of the largest number inside the vector y is the class ID of your_data_vector.
 * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.
 */

#define label_1_row 4
#define label_1_column 5

const static float label_1_w[label_1_row * label_1_column] = { 2.037219238e-03f, -5.191075616e-04f, 2.728153486e-03f, 2.521032700e-03f, 7.113998290e-03f, 
7.113998290e-03f, -4.102566093e-02f, -9.313225746e-09f, -1.025642082e-02f, -5.128210410e-03f, 
-5.128210410e-03f, -5.641028658e-02f, 1.262589917e-02f, 5.902571976e-02f, -1.887512393e-03f, 
-1.887512393e-03f, -4.589217715e-03f, -5.469211377e-03f, 3.282281011e-02f, -3.546598554e-02f };

const static float label_1_b[label_1_row] = { -1.372474432e+00f, 6.410260201e-01f, -1.250734329e+00f, 1.240374446e-01f };

const static ACTIVATION_FUNCTION label_1_activtion_function = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;

#endif /* !label_1_H_ */
