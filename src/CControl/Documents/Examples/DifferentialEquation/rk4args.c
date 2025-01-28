/*
 ============================================================================
 Name        : rk4args.c
 Author      : Daniel M�rtensson
 Version     : 1.0
 Copyright   : MIT
 Description : Fourth Order Runge-Kutta differential equation solver with variable sized arguments
 ============================================================================
 */

#include "CControl/ccontrol.h"

 /* Constants */
#define N 2
#define h 0.1f
#define row_a 2
#define column_a 2
#define row_b 2
#define column_b 1
#define row_u 1
#define column_u 1
#define ITERATIONS 200

/* Create ODE equation */
void odefun(float t, float y[], const float* matrices[], const size_t rows[], const size_t columns[]) {
    /* Get the matrices */
    float* A = matrices[0];
    float* B = matrices[1];
    float* u = matrices[2];

    /* Get the sizes */
    size_t row_A = rows[0];
    size_t column_A = columns[0];
    size_t row_B = rows[1];
    size_t column_B = columns[1];
    size_t row_U = rows[2];
    size_t column_U = columns[2];

    /* Solve y = A*y + B*u */
    float Ay[2];
    float Bu[2];
    mul(A, y, Ay, row_A, column_A, 1);
    mul(B, u, Bu, row_B, column_B, 1);
    size_t i;
    for (i = 0; i < row_A; i++) {
        y[i] = Ay[i] + Bu[i];
    }
}

int main() {
    clock_t start, end;
    float cpu_time_used;
    start = clock();

    /* Declare intial state */
    float y0[N] = { 0 };

    /* Declare output */
    float Y[ITERATIONS * N];

    /* Declare number of pointers */
    const size_t number_of_pointers = 3;
    const float k = 5.0f;    /* Spring constant [N/m] */
    const float b = 1.4f;    /* Damper constant [Ns/m] */
    const float m = 2.0f;    /* Mass [kg] */
    const float F = 8.0f;    /* Force [N] */
    const float A[row_a * column_a] = { 0, 1, -k / m, -b / m };
    const float B[row_b * column_b] = { 0, 1 / m };
    const float u[row_u * column_u] = { F };

    /* Perform Runge-Kutta 4:th order with extended arguments ... */
    rk4args(ITERATIONS, h, Y, y0, N, odefun, number_of_pointers, A, row_a, column_a, B, row_b, column_b, u, row_u, column_u);

    end = clock();
    cpu_time_used = ((float)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTotal speed  was %f\n", cpu_time_used);

    /* Plot */
    print(Y, ITERATIONS, N);

    /* Check memory */
    detectmemoryleak();

    return EXIT_SUCCESS;
}

/*
 GNU Octave code:

 function test2()
    F = 8;
    m = 2;
    b = 1.4;
    k = 5;

    % Definiera systemparametrar
    A = [0 1; -k/m -b/m];      % Tillst�ndsmatris
    B = [0; 1/m];              % Ing�ngsmatris
    C = [1 0];                 % Utmatris
    u = F;                     % Fixerad ing�ng
    x0 = [0; 0];               % Initialt tillst�nd

    % Tidsparametrar
    dt = 0.1;                  % Stegl�ngd
    N = 200;
    T_end = dt * N;            % Sluttid

    % Preallokera vektorer f�r tillst�nd och utmatning
    x = zeros(2, N+1);         % Tillst�ndet x vid varje tidssteg
    y = zeros(1, N+1);         % Utmatningen y vid varje tidssteg
    x(:, 1) = x0;              % Initialt tillst�nd
    y(:, 1) = C * x0;          % Initial utmatning

    % Definiera systemets dynamik som en funktion (A*x + B*u)
    dyn_func = @(x, pvi) A * pvi + B * u;

    % Anv�nd Runge-Kutta f�r att uppdatera tillst�ndet (RK4)
    for k = 1:N
        x(:, k+1) = rk4(dyn_func, k, x(:, k), dt); % RK4-steget
        y(:, k+1) = C * x(:, k+1);  % Ber�kna utmatningen y
    end

    % Anv�nd ode45 f�r att l�sa systemet
    ode_func = @(t, x) A * x + B * u;  % Dynamikfunktion f�r ode45
    [T_ode45, X_ode45] = ode45(ode_func, [0 T_end], x0);  % Anv�nd ode45
    Y_ode45 = C * X_ode45';  % Ber�kna utmatningen f�r ode45

    % Visualisera resultatet
    time = 1:length(x(1, :));

    figure;
    subplot(3,1,1);
    plot(time, x(1,:), time, x(2,:));
    xlabel('Tid (s)');
    ylabel('Tillst�nd');
    legend('x_1 (RK4)', 'x_2 (RK4)');
    title('Tillst�ndsvariabler med Runge-Kutta (RK4)');

    subplot(3,1,2);
    plot(T_ode45, X_ode45(:, 1), T_ode45, X_ode45(:, 2));
    xlabel('Tid (s)');
    ylabel('Tillst�nd');
    legend('x_1 (ode45)', 'x_2 (ode45)');
    title('Tillst�ndsvariabler med ode45');

    subplot(3,1,3);
    plot(time, y, T_ode45, Y_ode45);
    xlabel('Tid (s)');
    ylabel('Utmatning y');
    legend('y (RK4)', 'y (ode45)');
    title('Utmatning y (J�mf�relse mellan RK4 och ode45)');
end

function temp = rk4(func, x, pvi, h)
    K1 = h * func(x, pvi);
    K2 = h * func(x + 0.5 * h, pvi + 0.5 * K1);
    K3 = h * func(x + 0.5 * h, pvi + 0.5 * K2);
    K4 = h * func(x + h, pvi + K3);
    temp = pvi + (K1 + 2 * K2 + 2 * K3 + K4) / 6;
end

*/