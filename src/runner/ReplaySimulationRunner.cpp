#include "ReplaySimulationRunner.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "../io/MeasurementReader.h"
#include "../io/SimulationLogger.h"
#include "../solver/TimeIntegrator.h"

void ReplaySimulationRunner::run(const std::string& measurementFile, const std::string& outputFile, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, 
	const SimulationConfig& config, const std::vector<double>& initialTemperature, int centerNodeId, double maxTimeStep)
{

	if (!std::isfinite(maxTimeStep) || maxTimeStep <= 0.0)
	{
		throw std::runtime_error("Maximum time step must be positive and finite.");
	}


	MeasurementReader reader(measurementFile);
	SimulationLogger logger(outputFile);

	std::vector<double> currentTemperature = initialTemperature;

	if (!reader.hasNext())
	{
		throw std::runtime_error("No measurement data found.");
	}

	MeasurementSample initialSample = reader.next();

	double previousTime = initialSample.timeSeconds;

	double initalTMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());
	double initalTMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

	logger.logStep(initialSample.timeSeconds, initialSample.furnaceTemperature, currentTemperature[centerNodeId], initalTMin, initalTMax);



    MeasurementSample previousSample = initialSample;

    while (reader.hasNext())
    {
        MeasurementSample sample = reader.next();

        // Time interval between consecutive measurements
        const double interval = sample.timeSeconds - previousSample.timeSeconds;

        if (!std::isfinite(interval) || interval <= 0.0)
        {
            throw std::runtime_error("Measurement timestamps must be strictly increasing.");
        }

        // Number of internal FEM steps
        const int substeps = static_cast<int>(std::ceil(interval / maxTimeStep));

       

        const double dt = interval / substeps;

        for (int i = 1; i <= substeps; ++i)
        {
            // Linear interpolation of furnace temperature
            const double alpha = static_cast<double>(i) / substeps;

            const double furnaceTemperature = previousSample.furnaceTemperature + alpha * (sample.furnaceTemperature - previousSample.furnaceTemperature);

            auto stepResult = TimeIntegrator::stepNonLinearWithHTC(mesh, ue, material, currentTemperature, dt, 
                boundaryConditions, furnaceTemperature, config.maxPicardIterations, config.picardTolerance);

            currentTemperature = std::move(stepResult.temperature);
        }

        // Save results only at original measurement timestamps
        const double tMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());

        const double tMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

        logger.logStep(sample.timeSeconds, sample.furnaceTemperature, currentTemperature[centerNodeId], tMin, tMax);

        previousSample = sample;
    }

}
