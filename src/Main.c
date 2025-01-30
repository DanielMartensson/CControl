/*
 ============================================================================
 Name        : mpc.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Model Predictive Control with integral action
 ============================================================================
 */

#include "CControl/ccontrol.h"

 /* Constants */
#define SAMPLETIME 0.5f
#define row_a 2
#define column_b 1
#define row_c 1
#define column_e 1
#define ITERATIONS 100
#define N 5

int main() {
    clock_t start, end;
    float cpu_time_used;
    start = clock();

    /* Mass damping spring system of second order. Described as state space x(k+1) = A*x(k) + B*u(k) */
    const float k = 1.0f;    /* Spring constant [N/m] */
    const float b = 10.4f;   /* Damper constant [Ns/m] */
    const float m = 1.0f;    /* Mass [kg] */
    const float A[row_a * row_a] = { 0, 1, -k / m, -b / m };
    const float B[row_a * column_b] = { 0, 1/m };
    const float C[row_c * row_a] = { 1, 0 };
    const float E[row_a * column_e] = { 0, 1/m };

    /* Create discrete matrices */
    float Ad[row_a * row_a];
    float Bd[row_a * column_b];
    float Cd[row_c * row_a];
    float Ed[row_a * column_e];
    mpc_discrete_matrices(SAMPLETIME, A, B, C, E, Ad, Bd, Cd, Ed, row_a, column_b, row_c, column_e);

    /* Debug */
    print(Ad, row_a, row_a);
    print(Bd, row_a, column_b);
    print(Cd, row_c, row_a);
    print(Ed, row_a, column_e);

    /* Compute kalman gain */
    const float Q[row_a * row_a] = { 1, 0, 0, 1 };
    const float R[column_b * column_b] = { 1 };
    float K[row_a * row_c];
    mpc_kalman_gain(ITERATIONS, SAMPLETIME, Ad, Cd, Q, R, K, row_a, row_c);

    /* Debug */
    print(K, row_a, row_c);

    /* Observability matrix */
    float Phi[(N * row_c) * row_a];
    mpc_phi_matrix(Phi, Ad, Cd, row_a, row_c, N);

    /* Debug */
    print(Phi, N * row_c, row_a);

    /* Lower triangular toeplitz of extended observability matrix */
    float Gamma[(N * row_c) * (N * column_b)];
    mpc_gamma_matrix(Gamma, Phi, Bd, Cd, row_a, row_c, column_b, N);

    /* Debug */
    print(Gamma, N * row_c, N * column_b);

    /* Reference vector */
    float R[N * row_c];
    const float r[row_c] = { 10 };
    mpc_vector(R, r, N * row_c, 1);

    /* Debug */
    print(R, N, row_c);

    /* Weight matrix */
    float QZ[(N * row_c) * (N * row_c)];
    const float Qz[row_c * row_c] = { 1 };
    mpc_QZ_matrix(QZ, Qz, row_c, N);

    /* Debug */
    print(QZ, N * row_c, N * row_c);

    /* Regularization matrix */
    float S[column_b * column_b];
    const float s = 1.0f;
    mpc_S_matrix(S, s, column_b, column_b);
    float HS[(N * column_b) * (N * column_b)];
    mpc_HS_matrix(HS, S, column_b, N);

    /* Debug */
    print(S, column_b, column_b);
    print(HS, N * column_b, N * column_b);

    /* Create the QP solver H matrix */
    float H[(N * column_b) * (N * column_b)];
    mpc_H_matrix(H, Gamma, QZ, HS, row_c, column_b, N);

    /* Debug */
    print(H, N * column_b, N * column_b);

    /* Create the lower hankel toeplitz Gamma matrix of disturbance and its disturbance vector */
    float Gammad[(N * row_c) * (N * column_e)];
    mpc_gamma_matrix(Gamma, Phi, E, C, row_a, row_c, column_e, N);
    float D[N * column_e];
    float d[column_e] = { 0 };
    mpc_vector(D, d, column_e, N);

    /* Debug */
    print(Gammad, N * row_c, N * column_e);
    print(D, N * column_e, 1);

    /* Create the QP solver matrix for the gradient */
    float Mx0[(N * column_b) * row_a];
    mpc_Mx0_matrix(Mx0, Gamma, QZ, Phi, row_a, row_c, column_b, N);
    float Mum1[(N * column_b) * column_b];
    mpc_Mum1_matrix(Mum1, S, column_b, N);
    float MR[(N * column_b) * (N * column_b)];
    mpc_MR_matrix(MR, Gamma, QZ, row_c, column_b, N);
    float MD[(N * column_b) * (N * column_b)];
    mpc_MD_matrix(MD, Gamma, Gammad, QZ, row_c, column_b, N);

    /* Debug */
    print(Mx0, N * column_b, row_a);
    print(Mum1, N * column_b, column_b);
    print(MR, N * column_b, N * column_b);
    print(MD, N* column_b, N* column_b);

    /* Create constraints on the movment */
    float deltaUmin[(N - 1) * column_b];
    const float Umin[column_b] = { 1 };
    mpc_vector(deltaUmin, Umin, column_b, N - 1);
    float deltaUmax[(N - 1) * column_b];
    const float Umax[column_b] = { 1 };
    mpc_vector(deltaUmax, Umax, column_b, N - 1);
    float Lambda[((N - 1) * column_b) * column_b];
    mpc_Lambda_matrix(Lambda, column_b, N);

    /* Debug */
    print(deltaUmin, N - 1, column_b);
    print(deltaUmax, N - 1, column_b);
    print(Lambda, (N - 1) * column_b, column_b);

    /* Create constraints on outputs */
    float Zmin[N * row_c];
    const float zmin[row_c] = { 1 };
    mpc_vector(Zmin, zmin, row_c, N);
    float Zmax[N * row_c];
    const float zmax[row_c] = { 1 };
    mpc_vector(Zmax, zmax, row_c, N);

    /* Debug */
    print(Zmin, N * row_c, row_c);
    print(Zmax, N * row_c, row_c);

    /* Create the slack variables */
    float barSpsi[(N * column_b) * (N * column_b)];
    const float Spsi_spsi = 1;
    const float Spsi = Spsi_spsi;
    mpc_barSpsi_matrix(barSpsi, Spsi, column_b, N);
    float barspsi[N];
    const float spsi = Spsi_spsi;
    mpc_barspsi_vector(barspsi, spsi, N);

    /* Debug */
    print(barSpsi, N * column_b, N * column_b);
    print(barspsi, N, 1);

    /* Create QP solver matrix */
    float barH[(2 * N * column_b) * (2 * N * column_b)];
    mpc_barH_matrix(barH, H, barSpsi, column_b, N);

    end = clock();
    cpu_time_used = ((float)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTotal speed  was %f\n", cpu_time_used);


    /* Check memory */
    detectmemoryleak();

    return EXIT_SUCCESS;
}

/*
 GNU Octave code:

m = 1;
b = 10.4;
k = 1;

% Definiera systemparametrar
A = [0 1; -k/m -b/m];      % Tillståndsmatris
B = [0, k 1; m -1/m b/m ];          % Ingångsmatris
C = [1 0; 0 1];

pkg load control

sys = mc.ss(0, A, B, C);
sys1 = ss(A, B, C);
sysd = mc.c2d(sys, 0.5);
sysd1 = c2d(sys1, 0.5);


Q = [1 0; 0 1];
R = [ 1, 0, 0; 0, 0.5, 0; 0, 0, 3 ];

%[X, L, K] = dare(A, B, Q, R)
%[X, K, L] = mc.are(sysd, Q, R)
L = lqr(sysd1, Q, R)
L = mc.lqr(sysd, Q, R)

*/