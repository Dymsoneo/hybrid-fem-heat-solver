#include <iostream>
#include <algorithm>

#include "io/MeasurementReader.h"
#include "io/SimulationLogger.h"
#include "solver/TimeIntegrator.h"
#include "boundary/BoundaryConditionManager.h"
#include "boundary/HTCModel.h"
#include "config/SimulationConfig.h"
#include "mesh/MeshGenerator.h"

int main()
{
   
	MeasurementReader reader("data/experiments/experiment_001/measurements.csv");

	SimulationLogger simulationLogger("data/experiments/experiment_001/realtime_simulation.csv");

	SimulationConfig config = createRealtimeConfig();

	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);

	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	BoundaryConditionManager boundaryConditions;

	HTCModel furnanceHTC("data/boundary/htc_effective.csv");
	
	boundaryConditions.setHTCModel(BoundarySide::Right, furnanceHTC);
	//boundaryConditions.setHTCModel(BoundarySide::Top, furnanceHTC);

	std::vector<double> currentTemperature(mesh.nodesCount, 20.0);

	int centerNode = mesh.findClosestNode(0.0, 0.025);

	if (!reader.hasNext())
	{
		throw std::runtime_error("No measurement data found.");
	}

	MeasurementSample initialSample = reader.next();

	double initialAmbientTemperature = initialSample.furnaceTemperature;

	double initialMinTemperature = *std::min_element(currentTemperature.begin(), currentTemperature.end());
	double initialMaxTemperature = *std::max_element(currentTemperature.begin(), currentTemperature.end());

	simulationLogger.logStep(initialSample.timeSeconds, initialAmbientTemperature, currentTemperature[centerNode], initialMinTemperature, initialMaxTemperature);

	while (reader.hasNext())
	{
		MeasurementSample sample = reader.next();

		double ambientTemperature = sample.furnaceTemperature;

		auto stepResult = TimeIntegrator::stepNonLinearWithHTC(mesh, ue, material, currentTemperature, config.timeStep, boundaryConditions, ambientTemperature, config.maxPicardIterations, config.picardTolerance);

		currentTemperature = stepResult.temperature;

		double tMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());
		double tMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

		double centerTemperature = currentTemperature[centerNode];

		simulationLogger.logStep(sample.timeSeconds, ambientTemperature, centerTemperature, tMin, tMax);

	}

    return 0;
}