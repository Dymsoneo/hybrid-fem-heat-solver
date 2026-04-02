#include "SimulationRunner.h"
#include "TimeIntegrator.h"

SimulationRunner::SimulationResult SimulationRunner::runSimulation(const Assembly::AssemblyResult& system, const Vector& initialTemperature, double timeStep, double totalTime)
{
	SimulationResult result;

	Vector currentTemperature = initialTemperature;

	result.temperatureHistory.push_back(currentTemperature);
	result.timePoints.push_back(0.0);

	int steps = static_cast<int>(totalTime / timeStep);

	for (int step = 0; step < steps; ++step)
	{
		Vector nextTemperature = TimeIntegrator::step(system, currentTemperature, timeStep);

		currentTemperature = nextTemperature;

		result.temperatureHistory.push_back(currentTemperature);
		result.timePoints.push_back((step + 1) * timeStep);	
	}

	return result;
}
