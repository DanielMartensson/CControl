#ifndef CONTROL_ENGINEERING_H_
#define CONTROL_ENGINEERING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void c2d(float A[], float B[], const size_t row_a, const size_t column_b, const float sampleTime);
void cab(float GAMMA[], const float PHI[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N);
void qmpc(const float GAMMA[], const float PHI[], const float x[], float u[], const float Umax[], const float S[], const float r[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N, const float lambda, const bool has_integration_action, const float integration_constant);
void obsv(float PHI[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N);
bool ssint(const float A[], const float B[], const float C[], float Ai[], float Bi[], float Ci[], const size_t row_a, const size_t column_b, const size_t row_c);
bool stability(const float A[], const size_t row);

#ifdef __cplusplus
}
#endif

#endif /* !CONTROL_ENGINEERING_H_ */
