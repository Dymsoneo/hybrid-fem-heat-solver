#pragma once

#include<vector>

#include "../fem/Assembly.h"

class SimulationRunner
{
public:

	using Vector = std::vector<double>;
	using TemperatureHistory = std::vector<Vector>;

	struct SimulationResult
	{
		TemperatureHistory temperatureHistory;
		std::vector<double> timePoints;
	};

	static SimulationResult runSimulation(const Assembly::AssemblyResult& system, const Vector& initialTemperature, double timeStep, double totalTime);
};