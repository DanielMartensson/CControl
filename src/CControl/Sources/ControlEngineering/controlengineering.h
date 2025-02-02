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
void mpc_MD_matrix(float MD[], const float Gamma[], const float Gammad[], const float QZ[], const size_t row_c, const size_t column_b, const size_t column_e, const size_t N);
void mpc_Lambda_matrix(float Lambda[], const size_t column_b, const size_t N);
void mpc_barSpsi_matrix(float barSpsi[], const float Spsi, const size_t column_b, const size_t N);
void mpc_barspsi_vector(float barspsi[], const float spsi, const size_t column_b, const size_t N);
void mpc_barH_matrix(float barH[], const float H[], const float barSpsi[], const size_t column_b, const size_t N);
void mpc_eta_vector(float eta[], const float r[], const float y[], const float lambda, const size_t row_c);
void mpc_antiwindup_vector(float eta[], const float antiwindup, const size_t row_c);
void mpc_stateupdate_vector(float x[], const float Ad[], const float Bd[], const float Ed[], const float u[], const float d[], const size_t row_a, const size_t column_b, const size_t column_e);
void mpc_g_vector(float g[], const float Mx0[], const float x[], const float MR[], const float R[], const float eta[], const float MD[], const float D[], const float Mum1[], const float um1[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N);
void mpc_Umin_vector(float Umin[], const float umin[], const float deltaumin[], const float um1[], const size_t N, const size_t column_b);
void mpc_Umax_vector(float Umax[], const float umax[], const float deltaumax[], const float um1[], const size_t N, const size_t column_b);
void mpc_barZmin_vector(float barZmin[], const float Zmin[], const float Phi[], const float x0[], const float Gammad[], const float D[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N);
void mpc_barZmax_vector(float barZmax[], const float Zmax[], const float Phi[], const float x0[], const float Gammad[], const float D[], const size_t row_a, const size_t row_c, const size_t column_b, const size_t column_e, const size_t N);
void mpc_barg_vector(float barg[], const float g[], const float barspsi[], const size_t column_b, const size_t N);
void mpc_barUmin_vector(float barUmin[], const float Umin[], const size_t column_b, const size_t N);
void mpc_barUmax_vector(float barUmax[], const float Umax[], const size_t column_b, const size_t N);
void mpc_bmin_vector(float bmin[], const float deltaUmin[], const float barZmin[], const size_t column_b, const size_t row_c, const size_t N);
void mpc_bmax_vector(float bmin[], const float deltaUmax[], const float barZmax[], const size_t column_b, const size_t row_c, const size_t N);
void mpc_AA_matrix(float AA[], const float Lambda[], const float Gamma[], const size_t row_c, const size_t column_b, const size_t N);
void mpc_aqp_matrix(float aqp[], const float AA[], const size_t column_b, const size_t row_c, const size_t N);
void mpc_bqp_vector(float bqp[], const float barUmin[], const float barUmax[], const float bmin[], const float bmax[], const size_t column_b, const size_t row_c, const size_t N);


#ifdef __cplusplus
}
#endif

#endif /* !CONTROL_ENGINEERING_H_ */
