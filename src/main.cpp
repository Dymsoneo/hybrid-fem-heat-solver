#include <iostream>
#include <chrono>
#include <numeric>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/SimulationRunner.h"
#include "io/ResultExporter.h"
#include "material/MaterialModel.h"
#include "fem/ElementMatrices.h"
#include "solver/TimeIntegrator.h"
#include "config/SimulationConfig.h"


int main()
{
	//SimulationConfig config = createAccurateConfig();
	SimulationConfig config = createRealtimeConfig();

	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);
	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	std::vector<double> T(mesh.nodesCount, 20.0);

	double ambientTemperature = 400.0;

	double alpha = 25.0;

	auto result = SimulationRunner::runNonLinear(mesh, ue, material, T, config.timeStep, config.totalTime, alpha, ambientTemperature, config.picardTolerance, config.maxPicardIterations);

	int centerNode = mesh.findClosestNode(0.0, 0.025); 

	if (config.mode == SimulationMode::Realtime)
	{
		ResultExporter::exportSummary(result, "results/simulation_data/realtime_summary.csv", centerNode);
	}
	else
	{
		ResultExporter::exportSummary(result, "results/simulation_data/accurate_summary.csv", centerNode);
	}
	return 0;
}