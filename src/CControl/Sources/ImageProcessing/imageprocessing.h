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
void imresize(float X[], float Y[], float d, size_t row, size_t column);
void imgaussfilt(float X[], float sigma, size_t row, size_t column);
float* fspecial(const float value, size_t* output_size, const FSPECIAL_TYPE type);
HAARLIKE_FEATURE* haarlike_features(const size_t total_haar_likes, const uint8_t row, const uint8_t column);
int8_t haarlike_predict(const uint32_t X[], const HAARLIKE_FEATURE* feature, const uint8_t row, const uint8_t column);
void harris(const float X[], float H[], const float sigma, const uint8_t edge, const size_t row, const size_t column);
MODEL* imcollect(const MODEL_SETTINGS* model_settings);
void imcollectfree(MODEL* model);
void integralimage(const uint8_t A[], uint32_t B[], const size_t row, const size_t column);
float intensitycentroid(const float X[], const size_t radius, const size_t x, const size_t y, const size_t row, const size_t column);
uint32_t lbp(const uint8_t X[], const size_t row, const size_t column, const size_t x, const size_t y, const float init_angle, const float radius, LBP_BIT lbp_bit);
void pooling(float X[], float P[], size_t row, size_t column, uint8_t p, POOLING_METHOD pooling_method);
void rpca(float X[], float L[], float S[], size_t row, size_t column);
void sobel(const float X[], float G[], float O[], const size_t row, const size_t column, const SOBEL_METHOD sobel_method);

#ifdef __cplusplus
}
#endif

#endif /* !IMAGE_PROCESSING_H_ */
