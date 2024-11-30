#ifndef SYSTEM_IDENTIFICATION_H_
#define SYSTEM_IDENTIFICATION_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void rls(size_t NP, size_t NZ, size_t NZE, float theta[], float u, float y, size_t* count, float* past_e, float* past_y, float* past_u, float phi[], float P[], float Pq, float forgetting);
void okid(float u[], float y[], float g[], size_t row, size_t column);
void era(float u[], float y[], size_t row, size_t column, float A[], float B[], float C[], size_t row_a, size_t inputs_outputs);
void sr_ukf_parameter_estimation(const float d[], float what[], const float Re[], const float x[], void (*G)(float[], const float[], const float[]), const float lambda_rls, float Sw[], const float alpha, const float beta, const size_t L);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], size_t row_a, size_t NP, size_t NZ, bool integral_action);

#ifdef __cplusplus
}
#endif

#endif /* !SYSTEM_IDENTIFICATION_H_ */
