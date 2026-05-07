#include <iostream>

#include "boundary/HTCModel.h"
#include "boundary/BoundaryConditionManager.h"
#include "solver/SimulationRunner.h"
#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "io/ResultExporter.h"

int main()
{
    SimulationConfig config = createRealtimeConfig();

    Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);
    UniversalElement ue;

    MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	std::vector<double> initialTemperature(mesh.nodesCount, 20.0);

    double ambientTemperature = 400.0;

    BoundaryConditionManager boundaryConditions;
	HTCModel furnanceHTC("data/boundary/htc_effective.csv");
	HTCModel contactHTC(80.0);

    boundaryConditions.setHTCModel(BoundarySide::Right, furnanceHTC);
	boundaryConditions.setHTCModel(BoundarySide::Bottom, contactHTC);


    auto result = SimulationRunner::runNonLinearWithHTC(mesh, ue, material, initialTemperature, config.timeStep, config.totalTime, ambientTemperature, boundaryConditions, config.picardTolerance, config.maxPicardIterations);

	int centerNodeIndex = mesh.findClosestNode(0.0, 0.025);

	ResultExporter::exportSummary(result, "results/simulation_data/HTCModel_contact_summary.csv", centerNodeIndex);

    return 0;
}