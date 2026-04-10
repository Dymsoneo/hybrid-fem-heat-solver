#pragma once

#include<vector>

#include "../fem/Assembly.h"


// Runs a full transient FEM simulation over multiple time steps.
class SimulationRunner
{
public:

	using Vector = std::vector<double>;
	using TemperatureHistory = std::vector<Vector>;

	enum class SimulationMode
	{
		Linear,
		NonLinear
	};

	struct SimulationResult
	{
		TemperatureHistory temperatureHistory;
		std::vector<double> timePoints;
	};

	// Runs a linear transient simulation using a preassembled system.
	static SimulationResult runLinear(const Assembly::AssemblyResult& system, const Vector& initialTemperature, double timeStep, double totalTime);

	// Runs a nonlinear transient simulation, using Picard iteration in each time step.
	static SimulationResult runNonLinear(const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const Vector& initialTemperature, double timeStep, double totalTime, double alpha,
		double ambientTemperature, double tolerance, int maxIterations);
};