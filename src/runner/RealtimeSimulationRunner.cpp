#include "RealtimeSimulationRunner.h"

#include <chrono>

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "../io/SerialReader.h"
#include "../io/SimulationLogger.h"
#include "../solver/TimeIntegrator.h"
#include "../io/RealtimePerformanceLogger.h"

void RealtimeSimulationRunner::run(const std::string& portName, int baudRate, const std::string& outputFile, const std::string& performanceOutputFile, const Mesh& mesh,
	const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, const SimulationConfig& config, const std::vector<double>& initialTemperature, int centerNodeId, int maxSteps)
{
	if (maxSteps <= 0)
	{
		throw std::runtime_error("Max steps must be greater than 0");
	}

	SerialReader serialReader;
	serialReader.open(portName, baudRate);

	SimulationLogger logger(outputFile);
	
	RealtimePerformanceLogger performanceLogger(performanceOutputFile);

	std::vector<double> currentTemperature = initialTemperature;


	double furnaceTemperature = serialReader.readTemperature();

	double initialTMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());

	double initialTMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

	logger.logStep(0.0, furnaceTemperature, currentTemperature[centerNodeId], initialTMin, initialTMax);

	std::cout << "Initial furnace temperature: " << furnaceTemperature << " C" << std::endl;

	for (int step = 1; step <= maxSteps; step++)
	{
		furnaceTemperature = serialReader.readTemperature();

		auto startTime = std::chrono::steady_clock::now();

		auto stepResult = TimeIntegrator::stepNonLinearWithHTC(mesh, ue, material, currentTemperature, config.timeStep, boundaryConditions, furnaceTemperature, config.maxPicardIterations, config.picardTolerance);

		auto endTime = std::chrono::steady_clock::now();

		double computeTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

		double computeTimeSeconds = computeTimeMs / 1000.0;

		double realtimeFactor = config.timeStep / computeTimeSeconds;

		bool deadlineMissed = computeTimeSeconds > config.timeStep;

		currentTemperature = stepResult.temperature;

		double tMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());
		double tMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

		double currentTime = step * config.timeStep;

		logger.logStep(currentTime, furnaceTemperature, currentTemperature[centerNodeId], tMin, tMax);

		performanceLogger.logStep(step, currentTime, computeTimeMs, stepResult.stats.iterations, realtimeFactor, deadlineMissed);

		std::cout << "Step " << step << " | time = " << currentTime << " s | furnace = " << furnaceTemperature 
			<< " C | center = " << currentTemperature[centerNodeId] << " C | Tmax = " << tMax << " C | Picard iters = " << stepResult.stats.iterations << std::endl << "Compute time = " << computeTimeMs << " ms | RTF =  " << realtimeFactor << (deadlineMissed ? " | Deadline missed!" : "") << std::endl;
	}
}
