#pragma once

#include <vector>

#include "../fem/Assembly.h"

// Preforms one time step of transient heat conduction
class TimeIntegrator
{
public:
	using Matrix = std::vector<std::vector<double>>;
	using Vector = std::vector<double>;

	static Vector step(const Assembly::AssemblyResult& system, const Vector& Tn, double dt);
};