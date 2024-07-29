#ifndef CONTROL_ENGINEERING_H_
#define CONTROL_ENGINEERING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void c2d(float A[], float B[], const size_t row_a, const size_t column_b, const float sampleTime);
void cab(float GAMMA[], const float PHI[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N);
void qmpc(const float GAMMA[], const float PHI[], const float x[], float u[], const float Umax[], const float r[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N, const float lambda, const bool has_integration_action, const float integration_constant);
void kalman(float A[], float B[], float C[], float K[], float u[], float x[], float y[], size_t ADIM, size_t YDIM, size_t RDIM);
void lqi(float y[], float u[], float qi, float r[], float L[], float Li[], float x[], float xi[], size_t ADIM, size_t YDIM, size_t RDIM, ANTI_WINUP anti_winup);
void mrac(float limit, float gain, float y[], float u[], float r[], float I1[], float I2[], size_t RDIM);
void theta2ss(float A[], float B[], float C[], float theta[], float K[], size_t ADIM, size_t NP, size_t NZ, bool integral_action);
void obsv(float PHI[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N);
bool ssint(const float A[], const float B[], const float C[], float Ai[], float Bi[], float Ci[], const size_t row_a, const size_t column_b, const size_t row_c);
bool stability(const float A[], const size_t row);

#ifdef __cplusplus
}
#endif

#endif /* !CONTROL_ENGINEERING_H_ */
