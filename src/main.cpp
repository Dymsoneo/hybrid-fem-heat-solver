#include <iostream>


#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "runner/RealtimeSimulationRunner.h"


int main()
{

	SimulationConfig config = createRealtimeConfig();
	
	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);
	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	BoundaryConditionManager boundaryConditions;

	HTCModel furnaceHTC("data/boundary/htc_effective.csv");

	boundaryConditions.setHTCModel(BoundarySide::Right, furnaceHTC);
	boundaryConditions.setHTCModel(BoundarySide::Top, furnaceHTC);


	std::vector<double> initialTemperature(mesh.nodes.size(), 20.0);

	int centerNodeId = mesh.findClosestNode(0.0, 0.025);

	RealtimeSimulationRunner::run("COM7", 9600, "data/experiments/experiment_001/realtime_simulation_online.csv", mesh, ue, material, boundaryConditions, config, initialTemperature, centerNodeId, 50);

    return 0;
}