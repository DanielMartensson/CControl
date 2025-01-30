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
void mpc_discrete_matrices(const float sampleTime, const float A[], const float B[], const float C[], const float E[], float Ad[], float Bd[], float Cd[], float Ed[], const size_t row_a, const size_t column_b, const size_t row_c, const size_t column_e);
void mpc_kalman_gain(const size_t iterations, const float sampleTime, const float A[], const float C[], const float Q[], const float R[], float K[], const size_t row_a, const size_t row_c);
void mpc_phi_matrix(float Phi[], const float A[], const float C[], const size_t row_a, const size_t row_c, const size_t N);
void mpc_gamma_matrix(float Gamma[], const float Phi[], const float B[], const float C[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N);
void mpc_vector(float V[], const float v[], const size_t dim_v, const size_t N);
void mpc_QZ_matrix(float QZ[], const float Qz[], const size_t row_c, const size_t N);
void mpc_S_matrix(float S[], const float s, const size_t column_b);
void mpc_HS_matrix(float HS[], const float S[], const size_t column_b, const size_t N);
void mpc_H_matrix(float H[], const float Gamma[], const float QZ[], const float HS[], const size_t row_c, const size_t column_b, const size_t N);
void mpc_Mx0_matrix(float Mx0[], const float Gamma[], const float QZ[], const float Phi[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t N);
void mpc_Mum1_matrix(float Mum1[], const float S[], const size_t column_b, const size_t N);
void mpc_MR_matrix(float MR[], const float Gamma[], const float QZ[], const size_t row_c, const size_t column_b, const size_t N);
void mpc_MD_matrix(float MD[], const float Gamma[], const float Gammad[], const float QZ[], const size_t row_c, const size_t column_b, const size_t N);
void mpc_Lambda_matrix(float Lambda[], const size_t column_b, const size_t N);
void mpc_barSpsi_matrix(float barSpsi[], const float Spsi, const size_t column_b, const size_t N);
void mpc_barspsi_vector(float barspsi[], const float spsi, const size_t N);
void mpc_barH_matrix(float barH[], const float H[], const float barSpsi[], const size_t column_b, const size_t N);

#ifdef __cplusplus
}
#endif

#endif /* !CONTROL_ENGINEERING_H_ */
