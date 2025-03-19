#ifndef CONTROL_ENGINEERING_H_
#define CONTROL_ENGINEERING_H_

#include "../../Headers/headers.h"

#ifdef __cplusplus
extern "C" {
#endif

void c2d(float A[], float B[], const size_t row_a, const size_t column_b, const float sampleTime);
void cab(float Gamma[], const float Phi[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N);
void dare(const size_t iterations, const float sampleTime, const float A[], const float B[], const float Q[], const float R[], float X[], const size_t row_a, const size_t column_b);
bool kf(const float A[], const float B[], const float C[], const float u[], const float y[], const float Q[], const float R[], float xhat[], float P[], const size_t row_a, const size_t row_c, const size_t column_b);
void lqe(const size_t iterations, const float sampleTime, const float A[], const float C[], const float Q[], const float R[], float K[], const size_t row_a, const size_t row_c);
void lqr(const size_t iterations, const float sampleTime, const float A[], const float B[], const float Q[], const float R[], float L[], const size_t row_a, const size_t column_b);
bool qmpc(const float GAMMA[], const float PHI[], const float x[], float u[], const float Umax[], const float S[], const float r[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N, const float lambda, const bool has_integration_action, const float integration_constant);
void obsv(float Phi[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N);
bool ssint(const float A[], const float B[], const float C[], float Ai[], float Bi[], float Ci[], const size_t row_a, const size_t column_b, const size_t row_c);
bool stability(const float A[], const size_t row);

/* MPC */
bool mpc_init(MPC* mpc, const float A[], const float B[], const float C[], const float E[], const float sampleTime_mpc, const float sampleTime_kf, const float qw, const float rv, const float qz, const float s, const float Spsi_spsi, const size_t row_a, const size_t column_b, const size_t row_c, const size_t column_e, const size_t N, const size_t iterations);
void mpc_set_constraints(MPC* mpc, const float umin[], const float umax[], const float zmin[], const float zmax[], const float deltaumin[], const float deltaumax[], const float alpha, const float antiwindup);
STATUS_CODES mpc_optimize(MPC* mpc, float u[], const float r[], const float y[], const float d[], const bool integral_active);
void mpc_estimate(MPC* mpc, const float u[], const float y[], const float d[]);
bool mpc_free(MPC* mpc);


#ifdef __cplusplus
}
#endif

#endif /* !CONTROL_ENGINEERING_H_ */
