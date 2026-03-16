#include "LinearSolver.h"

#include <stdexcept>

LinearSolver::Vector LinearSolver::solve(Matrix A, Vector b)
{
    int n = b.size();

    for (int k = 0; k < n; k++)
    {
        if (A[k][k] == 0.0)
        {
            throw std::runtime_error("Zero pivot encountered");
        }

        for (int i = k + 1; i < n; i++)
        {
            double factor = A[i][k] / A[k][k];

            for (int j = k; j < n; j++)
            {
                A[i][j] -= factor * A[k][j];
            }

            b[i] -= factor * b[k];
        }
    }

    Vector x(n, 0.0);

    for (int i = n - 1; i >= 0; i--) 
    {
        double sum = b[i];
        for (int j = i + 1; j < n; j++)
        {
            sum -= A[i][j] * x[j];
        }

        x[i] = sum / A[i][i];
    }

    return x;
}
