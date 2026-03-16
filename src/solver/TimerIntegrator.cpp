#include "TimerIntegrator.h"
#include "LinearSolver.h"

TimeIntegrator::Vector TimeIntegrator::step(const Assembly::AssemblyResult& system, const Vector& Tn, double dt)
{
    int n = Tn.size();

    Matrix A(n, std::vector<double>(n, 0.0));
    Vector b(n, 0.0);


    // Building matrix A = C + dt(H + Hbc)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = system.C[i][j] + dt * (system.H[i][j] + system.Hbc[i][j]);
        }
    }

    // Building vector b = C*Tn + dt*P

    for (int i = 0; i < n; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < n; j++)
        {
            sum += system.C[i][j] * Tn[j];
        }

        b[i] = sum + dt * system.P[i];
    }

    // Solving linear system
    return LinearSolver::solve(A, b);
}
