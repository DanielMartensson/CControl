#ifndef COMPUTER_VISION_H_
#define COMPUTER_VISION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

COORDINATE_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);
COORDINATE_XY* featuredetection(const uint8_t X[], int* N, const int fast_threshold, const FAST_METHOD fast_method, const int row, const int column);
MODEL* fisherfaces(MODEL_SETTINGS* model_settings);
float* generalizedhough_collect(const uint8_t X[], const size_t row, const size_t column, const uint8_t fast_threshold, const FAST_METHOD fast_method, const float epsilon, const size_t min_pts, size_t* max_classes);
GENERALIZED_HOUGH_MODEL* generalizedhough_train(const float X[], const size_t row);
void generalizedhough_eval(const float X[], const GENERALIZED_HOUGH_MODEL model[], const float smoothing_accumulator, float* max_value_accumulator, size_t* xc, size_t* yc, const size_t row);
void generalizedhough_free(const GENERALIZED_HOUGH_MODEL model[]);
size_t hough(const float X[], float* K[], float* M[], const float p, const float epsilon, const size_t min_pts, const size_t row, const size_t column);
void voilajones_collect(uint32_t* data[], int8_t* y[], size_t* total_data_rows, const char train_path_objects[], const char train_path_non_objects[], const size_t row, const size_t column);
VIOLAJONES_MODEL* violajones_train(const VIOLAJONES_MODEL best_models[], const uint32_t X[], const int8_t y[], const size_t total_train_data_rows, const size_t total_haarlikes, const uint8_t N, const uint8_t row, const uint8_t column);
float violajones_eval(const VIOLAJONES_MODEL models[], const size_t N, const uint32_t X[], const int8_t y[], const size_t total_test_data_rows, const uint8_t row, const uint8_t column);
int8_t violajones_predict(const VIOLAJONES_MODEL models[], const size_t N, const uint32_t X[], const uint8_t row, const uint8_t column);

#ifdef __cplusplus
}
#endif

#endif /* !COMPUTER_VISION_H_ */