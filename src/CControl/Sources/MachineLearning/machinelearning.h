#ifndef MACHINE_LEARNING_H_
#define MACHINE_LEARNING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void centroid(const float X[], float C[], const size_t row, const size_t column);
float clusterratio(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
float clusterdistance(const float X[], const float Y[], const size_t row_x, const size_t row_y, const size_t column);
void dbscan(float X[], size_t idx[], float epsilon, size_t min_pts, size_t row, size_t column);
MODEL* fisherfaces(MODEL_SETTINGS* model_settings);
void kernel(const float X[], float K[], const size_t row, const size_t column, const float kernel_parameters[], const KERNEL_METHOD kernel_method);
bool kmeans(const float X[], size_t idx[], float C[], const size_t k, const size_t row, const size_t column);
void kpca(const float X[], float W[], float P[], const size_t c, const size_t row, const size_t column, const float kernel_parameters[], const KERNEL_METHOD kernel_method);
void kpca_lda_nn(MODEL_NN* model_nn, const MODEL_SETTINGS* model_settings);
uint32_t lbp(const float X[], const size_t row, const size_t column, const size_t x, const size_t y, const float init_angle, const float radius, LBP_BIT lbp_bit);
void nn_train(const float X[], const size_t class_id[], float weight[], float bias[], bool status[], float accuracy[], const size_t row, const size_t column, const size_t classes, const float C, const float lambda);
size_t nn_predict(const float model_w[], const float model_b[], const float x[], float y[], const size_t row_w, const size_t column_w, bool* class_id_found, const ACTIVATION_FUNCTION activation_function);
void nn_eval(const float model_w[], const float model_b[], const float X[], float Y[], const size_t class_id[], const size_t row_w, const size_t column_w, const size_t row_x, const ACTIVATION_FUNCTION activation_function);
void nn_save(const float model_w[], const float model_b[], const ACTIVATION_FUNCTION activation_function, const char model_path[], const char model_name[], const size_t row, const size_t column);
bool svm(float X[], float y[], float w[], float* b, float* accuracy, float C, float lambda, size_t row, size_t column);

#ifdef __cplusplus
}
#endif

#endif /* !MACHINE_LEARNING_H_ */