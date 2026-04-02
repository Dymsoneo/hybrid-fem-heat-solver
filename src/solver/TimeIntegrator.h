#pragma once

#include <vector>

#include "../fem/Assembly.h"
#include "../material/MaterialModel.h"
#include "../mesh/Mesh.h"
#include "../fem/UniversalElement.h"

// Preforms one time step of transient heat conduction
class TimeIntegrator
{
public:
	using Matrix = std::vector<std::vector<double>>;
	using Vector = std::vector<double>;

	// Linear implicit time integration step: (C + dt(H + Hbc)) * Tn+1 = C*Tn + dt*P
	static Vector step(const Assembly::AssemblyResult& system, const Vector& Tn, double dt);

	// Non-linear time integration step using material model: (C(Tn) + dt(H(Tn) + Hbc)) * Tn+1 = C(Tn)*Tn + dt*P
	static Vector stepNonLinear(const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const Vector& Tn, double dt, double alpha, double ambientTemperature, int maxIterations, double tolerance);
};