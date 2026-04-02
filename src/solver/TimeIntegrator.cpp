#include "TimeIntegrator.h"
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

TimeIntegrator::Vector TimeIntegrator::stepNonLinear(const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const Vector& Tn, double dt, double alpha, double ambientTemperature, int maxIterations, double tolerance)
{
    // Picard iteration for one nonlinear time step. Initial guess: T^(0) = T^n
    // For each iteration:
    // 1. assemble H(T) and C(T) using current guess  
    // 2. solve linear system
    // 3. check for convergence

    Vector currentGuess = Tn;
	Vector nextGuess = Tn;

    for (int iter = 0; iter < maxIterations; iter++)
    {
        auto system = Assembly::assembleSystemWithMaterialModel(mesh, ue, material, currentGuess, alpha, ambientTemperature);

        int n = static_cast<int>(Tn.size());

        Matrix A(n, std::vector<double>(n, 0.0));
		Vector b(n, 0.0);

		// A = C(T) + dt(H(T) + Hbc)
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                A[i][j] = system.C[i][j] + dt * (system.H[i][j] + system.Hbc[i][j]);
            }
        }

		// b = C(T)*Tn + dt*P
        for (int i = 0; i < n; i++)
        {
            double sum = 0.0;
            for (int j = 0; j < n; j++)
            {
				sum += system.C[i][j] * Tn[j];
            }
			b[i] = sum + dt * system.P[i];
        }

		nextGuess = LinearSolver::solve(A, b);

        // convergence check: max norm
		double maxDifference = 0.0;

        for (int i = 0; i < n; i++)
        {
			maxDifference = std::max(maxDifference, std::abs(nextGuess[i] - currentGuess[i]));
        }

		if (maxDifference < tolerance)
        {
			return nextGuess;
        }

        currentGuess = nextGuess;
    }

	return nextGuess; // Return last guess if max iterations reached without convergence
}
