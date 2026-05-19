#include <iostream>
#include <algorithm>

#include "io/MeasurementReader.h"
#include "io/SimulationLogger.h"
#include "solver/TimeIntegrator.h"
#include "boundary/BoundaryConditionManager.h"
#include "boundary/HTCModel.h"
#include "config/SimulationConfig.h"
#include "mesh/MeshGenerator.h"
#include "runner/ReplaySimulationRunner.h"

int main()
{
	SimulationConfig config = createRealtimeConfig();

	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);

	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	BoundaryConditionManager boundaryConditions;

	HTCModel furnanceHTC("data/boundary/htc_effective.csv");
	
	boundaryConditions.setHTCModel(BoundarySide::Right, furnanceHTC);
	//boundaryConditions.setHTCModel(BoundarySide::Top, furnanceHTC);

	std::vector<double> initialTemperature(mesh.nodesCount, 20.0);

	int centerNode = mesh.findClosestNode(0.0, 0.025);

	ReplaySimulationRunner::run("data/experiments/experiment_001/measurements.csv", "data/experiments/experiment_001/realtime_simulation.csv", mesh, ue, material, boundaryConditions, config, initialTemperature, centerNode);

    return 0;
}