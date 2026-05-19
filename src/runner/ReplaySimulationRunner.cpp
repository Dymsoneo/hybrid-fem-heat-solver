#include "ReplaySimulationRunner.h"

#include <algorithm>

#include "../io/MeasurementReader.h"
#include "../io/SimulationLogger.h"
#include "../solver/TimeIntegrator.h"

void ReplaySimulationRunner::run(const std::string& measurementFile, const std::string& outputFile, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, const SimulationConfig& config, const std::vector<double>& initialTemperature, int centerNodeId)
{
	MeasurementReader reader(measurementFile);
	SimulationLogger logger(outputFile);

	std::vector<double> currentTemperature = initialTemperature;

	if (!reader.hasNext())
	{
		throw std::runtime_error("No measurement data found.");
	}

	MeasurementSample initialSample = reader.next();

	double initalTMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());
	double initalTMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

	logger.logStep(initialSample.timeSeconds, initialSample.furnaceTemperature, currentTemperature[centerNodeId], initalTMin, initalTMax);


	while (reader.hasNext())
	{
		MeasurementSample sample = reader.next();

		auto stepResult = TimeIntegrator::stepNonLinearWithHTC(mesh, ue, material, currentTemperature, config.timeStep, boundaryConditions, sample.furnaceTemperature, config.maxPicardIterations, config.picardTolerance);

		currentTemperature = stepResult.temperature;

		double tMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());
		double tMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

		logger.logStep(sample.timeSeconds, sample.furnaceTemperature, currentTemperature[centerNodeId], tMin, tMax);
	}
}
