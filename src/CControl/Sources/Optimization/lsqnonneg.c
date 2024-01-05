/*
 * lsqnonneg.c
 *
 *  Created on: 24 april. 2023
 *      Author: Daniel Mårtensson
 */

#include "optimization.h"

/*
 * Non-Negative Least Squares (NNLS) Algorithm 
 * Min Ax - b
 * S.t x >= 0
 * 
 * A[m*n]
 * x[n]
 * b[m]
 * residual[m]
 */
bool lsqnonneg(float A[], float x[], float b[], float residual[], size_t row, size_t column) {
    /* Declare */
    size_t i, j, k, c;
    size_t* active_set = (size_t*)malloc(column * sizeof(size_t));
    size_t* inactive_set = (size_t*)malloc(column * sizeof(size_t));
    float* A_active = (float*)malloc(row * column * sizeof(float));
    float* x_new = (float*)malloc(column * sizeof(float));
    float* x_active = (float*)malloc(column * sizeof(float));
    float const tol = 1e-6f;
    float sum_value, max_val = 0.0f;
    size_t numbers_active = 0;
    size_t max_index;
    bool found;
    bool solution = false;

    /* Save address */
    float* A_active0 = A_active;
    float* A0 = A;

    for (c = 0; c < column; c++) {
        /* Clear */
        memset(x_new, 0, column * sizeof(float));

        /* Find active columns */
        if (numbers_active > 0) {
            /* Extract columns from A that are active */
            for (j = 0; j < numbers_active; j++) {
                A_active = A_active0;
                A = A0;
                for (i = 0; i < row; i++) {
                    A_active[j] = A[active_set[j]];

                    /* New row */
                    A_active += numbers_active;
                    A += column;
                }
            }
            A_active = A_active0;

            /* Solve linear equations system A_active*x_active = b */
            linsolve_gauss(A_active, x_active, b, row, numbers_active, 0.0f);

            /* Store new values in x_new */
            for (j = 0; j < numbers_active; j++) {
                x_new[active_set[j]] = x_active[j];
            }

        }

        /* Compute residual */
        A = A0;
        for (i = 0; i < row; i++) {
            residual[i] = b[i];
            for (j = 0; j < column; j++) {
                residual[i] -= A[j] * x_new[j];
            }
            A += column;
        }
        A = A0;

        /* Check which columns are not active */
        if (numbers_active > 0) {
            k = 0;
            for (i = 0; i < column; i++) {
                found = false;
                for (j = 0; j < numbers_active; j++) {
                    if (active_set[j] == i) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    inactive_set[k++] = i;
                }
            }
        }
        else {
            for (i = 0; i < column; i++) {
                inactive_set[i] = i;
            }
        }

        /* Compute optimality 
            B = A(:, inactive_set);
            C = B'*residual;
            [max_val, j] = max(C);
            max_index = inactive_set(j);
        
        */
        max_val = 0.0f;
        for (j = 0; j < column; j++) {
            sum_value = 0.0f;
            A = A0;
            for (i = 0; i < row; i++) {
                sum_value += A[inactive_set[j]] * residual[i];

                /* New row */
                A += column;
            }
            if (sum_value > max_val) {
                max_val = sum_value;
                max_index = inactive_set[j];
            }
        }

        /* Check if we have a solution */
        if (max_val < tol) {
            memcpy(x, x_new, column * sizeof(float));
            solution = true;
            break;
        }

        /* Add index max_index to active set */
        active_set[numbers_active] = max_index;

        /* Count active */
        numbers_active = numbers_active + 1;
    }

    /* Free */
    free(active_set);
    free(inactive_set);
    free(A_active);
    free(x_new);
    free(x_active);

    return solution;
}

/* GNU Octave code
    function [x, residual] = nnls(A, b)
      % Non-Negative Least Squares (NNLS) using Active Set Method
      % A: m x n matrix (m >= n), b: m x 1 vector
      % x: n x 1 vector, residual: scalar
      % Reference: Lawson and Hanson, "Solving Least Squares Problems", Prentice-Hall, 1974.

      [m, n] = size(A);
      x = zeros(n, 1);
      active_set = zeros(1, n); % indices of active columns in A
      inactive_set = zeros(1, n);
      tol = 1e-6; % tolerance for checking optimality
      residual = 0;
      numbers_active = 0;

      for c = 1:n
          % Clear
          x_new = zeros(n, 1);

          % Find active columns
          if(numbers_active > 0)
            % Extract columns from A that are active
            A_active = zeros(m, numbers_active);
            for i = 1:numbers_active
              A_active(:, i) = A(:, active_set(i));
            end

            % Solve linear equations system A_active*x_active = b
            x_active = linsolve(A_active, b);

            % Store new values in x_new
            for i = 1:numbers_active
                x_new(active_set(i)) = x_active(i);
            end
          end

          % Compute residual
          residual = b - A * x_new;

          % Check which columns are not active
          if(numbers_active > 0)
            %inactive_set = setdiff(1:n, active_set);

            k = 1;
            for i = 1:n
              found = false;
              for j = 1:numbers_active
                if(active_set(j) == i)
                  found = true;
                  break;
                end
              end
              if(~found)
                inactive_set(k) = i;
                k = k + 1;
              end
            end
          else
            for i = 1:n
              inactive_set(i) = i;
            end
          end

          % Compute optimality
          B = A(:, inactive_set);
          C = B'*residual;
          [max_val, j] = max(C);
          max_index = inactive_set(j);

          % Check if we have a solution
          if max_val < tol
              x = x_new;
              break;
          end

          % Count active
          numbers_active = numbers_active + 1;

          % Add index max_index to active set
          active_set(numbers_active) = max_index;
      end

    end

*/