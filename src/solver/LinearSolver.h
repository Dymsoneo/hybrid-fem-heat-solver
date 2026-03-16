#pragma once

#include <vector>

// Solves linear systems of equations (A x = b) using Gaussian elimination
class LinearSolver
{
public:

	using Matrix = std::vector<std::vector<double>>;
	using Vector = std::vector<double>;

	static Vector solve(Matrix A, Vector b);
};
