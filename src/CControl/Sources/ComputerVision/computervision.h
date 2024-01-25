#ifndef COMPUTER_VISION_H_
#define COMPUTER_VISION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

float* generalizedhough_create_data(const uint8_t X[], const size_t row, const size_t column, uint8_t* max_classes, const uint8_t descriptor_threshold, const uint32_t descriptors[], const uint8_t total_descriptors, const float epsilon, const size_t min_pts);
GENERALIZED_HOUGH_MODEL* generalizedhough_create_model(const float X[], const size_t row);
void generalizedhough_eval_votes(const float X[], const GENERALIZED_HOUGH_MODEL model[], const float smoothing_accumulator, float* max_value_accumulator, size_t* xc, size_t* yc, const size_t row);
void generalizedhough_free_model(const GENERALIZED_HOUGH_MODEL model[]);
size_t hough(const float X[], float* K[], float* M[], const float p, const float epsilon, const size_t min_pts, const size_t row, const size_t column);
FAST_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);
FAST_XY* featuredetection(const uint8_t X[], int* N, const int fast_threshold, const FAST_METHOD fast_method, const int row, const int column);
uint32_t landmarkdetection_analyze(const uint8_t X[], const size_t row, size_t column, const int x, const int y);
FAST_XY* landmarkdetection_collect(const uint8_t X[], int* N, const uint8_t descriptor_threshold, const uint32_t descriptors[], const uint8_t total_descriptors, const size_t row, const size_t column);
FAST_XY* shapedetection(const uint8_t X[], int* N, const uint8_t fast_threshold, const uint8_t sobel_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);

#ifdef __cplusplus
}
#endif

#endif /* !COMPUTER_VISION_H_ */