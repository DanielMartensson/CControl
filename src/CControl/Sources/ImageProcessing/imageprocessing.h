#ifndef IMAGE_PROCESSING_H_
#define IMAGE_PROCESSING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void imshow(PGM* image);
void imfree(PGM* image);
PGM* imread(const char file_path[]);
bool imwrite(const uint8_t X[], const char file_path[], const size_t row, const size_t column);
FAST_XY* shapedetection(const uint8_t X[], int* N, const uint8_t fast_threshold, const uint8_t sobel_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);
void imresize(float X[], float Y[], float d, size_t row, size_t column);
void imgaussfilt(float X[], float sigma, size_t row, size_t column);
FAST_XY* cornerdetection(const uint8_t X[], const float sigma, int* N, const uint8_t fast_threshold, const FAST_METHOD fast_method, const size_t row, const size_t column);
FAST_XY* featuredetection(const uint8_t X[], int* N, const int fast_threshold, const FAST_METHOD fast_method, const int row, const int column);
float* fspecial(const float value, size_t* output_size, const FSPECIAL_TYPE type);
float* generalizedhough_create_data(const uint8_t X[], const size_t row, const size_t column, uint8_t* max_classes, const uint8_t descriptor_threshold, const uint32_t descriptors[], const uint8_t total_descriptors, const float epsilon, const size_t min_pts);
GENERALIZED_HOUGH_MODEL* generalizedhough_create_model(const float X[], const size_t row);
void generalizedhough_eval_votes(const float X[], const GENERALIZED_HOUGH_MODEL model[], const float smoothing_accumulator, float* max_value_accumulator, size_t* xc, size_t* yc, const size_t row);
void generalizedhough_free_model(const GENERALIZED_HOUGH_MODEL model[]);
void harris(const float X[], float H[], const float sigma, const uint8_t edge, const size_t row, const size_t column);
size_t hough(const float X[], float* K[], float* M[], const float p, const float epsilon, const size_t min_pts, const size_t row, const size_t column);
MODEL* imcollect(const MODEL_SETTINGS* model_settings);
void imcollectfree(MODEL* model);
void integralimage(const float A[], float B[], const size_t row, const size_t column);
float intensitycentroid(const float X[], const size_t radius, const size_t x, const size_t y, const size_t row, const size_t column);
uint32_t landmarkdetection_analyze(const uint8_t X[], const size_t row, size_t column, const int x, const int y);
FAST_XY* landmarkdetection_collect(const uint8_t X[], int* N, const uint8_t descriptor_threshold, const uint32_t descriptors[], const uint8_t total_descriptors, const size_t row, const size_t column);
void pooling(float X[], float P[], size_t row, size_t column, uint8_t p, POOLING_METHOD pooling_method);
void rpca(float X[], float L[], float S[], size_t row, size_t column);
void sobel(const float X[], float G[], float O[], const size_t row, const size_t column, const SOBEL_METHOD sobel_method);

#ifdef __cplusplus
}
#endif

#endif /* !IMAGE_PROCESSING_H_ */
