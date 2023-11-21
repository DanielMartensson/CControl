/*
 * layer_0.h
 *
 * Created on : 2023-11-10 10:42:57
 * Author: <Your name>
 * Purpose: <Classify <your things>
 * Model ID: 30804
 */

#ifndef layer_0_H_
#define layer_0_H_

/* Add the CControl header file */
#include "ccontrol.h"

/*
 * This is a Neural Network - A model that can classify raw data.
 * Most used for images, but you can use it at any data you want as long the data is stored into .pgm files in format P2 or P5.
 * This file contains one matrix called 'layer_0_w' and one vector called 'layer_0_b'.
 * To use this header file, your data that must be a vector of size 'layer_0_column' in row-major.
 * For example, if you have an image with size m * n, where m * n = 'layer_0_column', then you need to turn it into a vector in row-wise, not in column-wise.
 * Multiply your vector together with the matrix 'layer_0_w' and add vector 'layer_0_b'. Then you will get a new vector called y with the size 'layer_0_row'.
 * y = layer_0_w*your_data_vector + layer_0_b
 * The index of the largest number inside the vector y is the class ID of your_data_vector.
 * If you get wrong class ID, then you need to use take account to noise or use better dimension reduction.
 */

#define layer_0_row 4
#define layer_0_column 88

const static float layer_0_w[layer_0_row * layer_0_column] = { 1.465961523e-02f, 1.465961523e-02f, 8.953851461e-02f, 8.953851461e-02f, 1.769770384e-01f, 5.893489718e-02f, -7.724132389e-02f, 2.017716505e-02f, -1.360612959e-01f, -7.330618799e-03f, 2.412991822e-01f, 7.960676402e-02f, -7.127308100e-02f, 6.796895713e-02f, -1.738205254e-01f, -1.205855310e-01f, 1.465961523e-02f, 1.465961523e-02f, 8.953851461e-02f, 8.953851461e-02f, 1.769770384e-01f, 5.893489718e-02f, 1.738501191e-01f, -2.181076072e-02f, 2.662399411e-02f, 1.531358212e-01f, 4.897730649e-01f, 1.975000203e-01f, 7.205239683e-02f, 3.775609285e-02f, -1.069907248e-01f, 6.644980609e-02f, 1.814356633e-02f, 1.814356633e-02f, 1.814356633e-02f, 9.302255511e-02f, -2.059355937e-02f, -7.480857521e-02f, -1.223546937e-01f, -1.232955828e-01f, 2.438423783e-01f, 1.929738373e-01f, 2.620801330e-01f, 7.005162537e-02f, 9.043150395e-02f, -6.137341261e-02f, -5.739405751e-01f, -1.920346022e-01f, 2.853954211e-03f, 2.853954211e-03f, 2.853954211e-03f, 7.773301005e-02f, 1.487125158e-01f, 1.458121240e-01f, 1.697583497e-01f, 5.582430214e-02f, -1.285531968e-01f, 4.744171351e-02f, 2.334174514e-01f, 3.306614608e-02f, -1.057165787e-01f, -1.935470104e-01f, -3.008378744e-01f, -7.866188884e-02f, 1.733938046e-02f, 4.802402854e-02f, 2.561474219e-03f, 2.561474219e-03f, -1.586565934e-02f, 1.080862582e-01f, -7.109761983e-02f, -1.072121188e-01f, 5.017504096e-03f, 5.262964219e-02f, 9.089963883e-02f, -2.735354006e-02f, -1.495805383e-01f, -1.605149508e-01f, -2.222779989e-01f, 5.920908600e-02f, 3.665855527e-02f, 3.665855527e-02f, -1.254010201e-01f, -1.516667008e-01f, 7.294321433e-03f, 2.361054905e-02f, -1.352204084e-01f, 1.377095878e-01f, 
1.769770384e-01f, 5.893489718e-02f, -7.724132389e-02f, 2.017716505e-02f, -1.360612959e-01f, -7.330618799e-03f, 2.412991822e-01f, 7.960676402e-02f, -7.127308100e-02f, 6.796895713e-02f, -1.738205254e-01f, -1.205855310e-01f, 1.465961523e-02f, 1.465961523e-02f, 8.953851461e-02f, 8.953851461e-02f, 1.769770384e-01f, 5.893489718e-02f, 1.738501191e-01f, -2.181076072e-02f, 2.662399411e-02f, 1.531358212e-01f, 4.897730649e-01f, 1.975000203e-01f, 7.205239683e-02f, 3.775609285e-02f, -1.069907248e-01f, 6.644980609e-02f, 1.814356633e-02f, 1.814356633e-02f, 1.814356633e-02f, 9.302255511e-02f, -2.059355937e-02f, -7.480857521e-02f, -1.223546937e-01f, -1.232955828e-01f, 2.438423783e-01f, 1.929738373e-01f, 2.620801330e-01f, 7.005162537e-02f, 9.043150395e-02f, -6.137341261e-02f, -5.739405751e-01f, -1.920346022e-01f, 2.853954211e-03f, 2.853954211e-03f, 2.853954211e-03f, 7.773301005e-02f, 1.487125158e-01f, 1.458121240e-01f, 1.697583497e-01f, 5.582430214e-02f, -1.285531968e-01f, 4.744171351e-02f, 2.334174514e-01f, 3.306614608e-02f, -1.057165787e-01f, -1.935470104e-01f, -3.008378744e-01f, -7.866188884e-02f, 1.733938046e-02f, 4.802402854e-02f, 2.561474219e-03f, 2.561474219e-03f, -1.586565934e-02f, 1.080862582e-01f, -7.109761983e-02f, -1.072121188e-01f, 5.017504096e-03f, 5.262964219e-02f, 9.089963883e-02f, -2.735354006e-02f, -1.495805383e-01f, -1.605149508e-01f, -2.222779989e-01f, 5.920908600e-02f, 3.665855527e-02f, 3.665855527e-02f, -1.254010201e-01f, -1.516667008e-01f, 7.294321433e-03f, 2.361054905e-02f, -1.352204084e-01f, 1.377095878e-01f, 4.257559776e-03f, 4.957556725e-02f, 1.020216197e-02f, -1.725457609e-03f, 
-1.360612959e-01f, -7.330618799e-03f, 2.412991822e-01f, 7.960676402e-02f, -7.127308100e-02f, 6.796895713e-02f, -1.738205254e-01f, -1.205855310e-01f, 1.465961523e-02f, 1.465961523e-02f, 8.953851461e-02f, 8.953851461e-02f, 1.769770384e-01f, 5.893489718e-02f, 1.738501191e-01f, -2.181076072e-02f, 2.662399411e-02f, 1.531358212e-01f, 4.897730649e-01f, 1.975000203e-01f, 7.205239683e-02f, 3.775609285e-02f, -1.069907248e-01f, 6.644980609e-02f, 1.814356633e-02f, 1.814356633e-02f, 1.814356633e-02f, 9.302255511e-02f, -2.059355937e-02f, -7.480857521e-02f, -1.223546937e-01f, -1.232955828e-01f, 2.438423783e-01f, 1.929738373e-01f, 2.620801330e-01f, 7.005162537e-02f, 9.043150395e-02f, -6.137341261e-02f, -5.739405751e-01f, -1.920346022e-01f, 2.853954211e-03f, 2.853954211e-03f, 2.853954211e-03f, 7.773301005e-02f, 1.487125158e-01f, 1.458121240e-01f, 1.697583497e-01f, 5.582430214e-02f, -1.285531968e-01f, 4.744171351e-02f, 2.334174514e-01f, 3.306614608e-02f, -1.057165787e-01f, -1.935470104e-01f, -3.008378744e-01f, -7.866188884e-02f, 1.733938046e-02f, 4.802402854e-02f, 2.561474219e-03f, 2.561474219e-03f, -1.586565934e-02f, 1.080862582e-01f, -7.109761983e-02f, -1.072121188e-01f, 5.017504096e-03f, 5.262964219e-02f, 9.089963883e-02f, -2.735354006e-02f, -1.495805383e-01f, -1.605149508e-01f, -2.222779989e-01f, 5.920908600e-02f, 3.665855527e-02f, 3.665855527e-02f, -1.254010201e-01f, -1.516667008e-01f, 7.294321433e-03f, 2.361054905e-02f, -1.352204084e-01f, 1.377095878e-01f, 4.257559776e-03f, 4.957556725e-02f, 1.020216197e-02f, -1.725457609e-03f, -1.095487773e-01f, -3.525579274e-01f, -1.863594651e-01f, -6.936439872e-02f, 
-7.127308100e-02f, 6.796895713e-02f, -1.738205254e-01f, -1.205855310e-01f, 1.465961523e-02f, 1.465961523e-02f, 8.953851461e-02f, 8.953851461e-02f, 1.769770384e-01f, 5.893489718e-02f, 1.738501191e-01f, -2.181076072e-02f, 2.662399411e-02f, 1.531358212e-01f, 4.897730649e-01f, 1.975000203e-01f, 7.205239683e-02f, 3.775609285e-02f, -1.069907248e-01f, 6.644980609e-02f, 1.814356633e-02f, 1.814356633e-02f, 1.814356633e-02f, 9.302255511e-02f, -2.059355937e-02f, -7.480857521e-02f, -1.223546937e-01f, -1.232955828e-01f, 2.438423783e-01f, 1.929738373e-01f, 2.620801330e-01f, 7.005162537e-02f, 9.043150395e-02f, -6.137341261e-02f, -5.739405751e-01f, -1.920346022e-01f, 2.853954211e-03f, 2.853954211e-03f, 2.853954211e-03f, 7.773301005e-02f, 1.487125158e-01f, 1.458121240e-01f, 1.697583497e-01f, 5.582430214e-02f, -1.285531968e-01f, 4.744171351e-02f, 2.334174514e-01f, 3.306614608e-02f, -1.057165787e-01f, -1.935470104e-01f, -3.008378744e-01f, -7.866188884e-02f, 1.733938046e-02f, 4.802402854e-02f, 2.561474219e-03f, 2.561474219e-03f, -1.586565934e-02f, 1.080862582e-01f, -7.109761983e-02f, -1.072121188e-01f, 5.017504096e-03f, 5.262964219e-02f, 9.089963883e-02f, -2.735354006e-02f, -1.495805383e-01f, -1.605149508e-01f, -2.222779989e-01f, 5.920908600e-02f, 3.665855527e-02f, 3.665855527e-02f, -1.254010201e-01f, -1.516667008e-01f, 7.294321433e-03f, 2.361054905e-02f, -1.352204084e-01f, 1.377095878e-01f, 4.257559776e-03f, 4.957556725e-02f, 1.020216197e-02f, -1.725457609e-03f, -1.095487773e-01f, -3.525579274e-01f, -1.863594651e-01f, -6.936439872e-02f, 9.577506781e-02f, 2.208326757e-02f, 2.208326757e-02f, 1.386572421e-02f };

const static float layer_0_b[layer_0_row] = { -2.189714193e+00f, -5.870319605e-01f, -5.734374046e+00f, 6.072473526e-01f };

const static ACTIVATION_FUNCTION layer_0_activtion_function = ACTIVATION_FUNCTION_CLOSEST_VALUE_INDEX;

#endif /* !layer_0_H_ */