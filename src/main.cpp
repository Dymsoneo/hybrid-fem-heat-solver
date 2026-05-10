#include <iostream>

#include "boundary/HTCModel.h"
#include "boundary/BoundaryConditionManager.h"
#include "solver/SimulationRunner.h"
#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "io/ResultExporter.h"

int main()
{
	SimulationConfig config = createAccurateConfig();

    Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);
    UniversalElement ue;

    MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	std::vector<double> initialTemperature(mesh.nodesCount, 20.0);

    double ambientTemperature = 400.0;

    BoundaryConditionManager boundaryConditions;

    HTCModel constantHTC(25.0);
    HTCModel variableHTC("data/boundary/htc_effective.csv");
    HTCModel contactHTC(80.0);

    // Case 1
	/*boundaryConditions.setHTCModel(BoundarySide::Right, constantHTC);
	boundaryConditions.setHTCModel(BoundarySide::Top, constantHTC);*/

    // Case 2
	/*boundaryConditions.setHTCModel(BoundarySide::Right, variableHTC);
	boundaryConditions.setHTCModel(BoundarySide::Top, variableHTC);*/

    // Case 3
	boundaryConditions.setHTCModel(BoundarySide::Right, variableHTC);
	boundaryConditions.setHTCModel(BoundarySide::Top, variableHTC);
	boundaryConditions.setHTCModel(BoundarySide::Bottom, contactHTC);

    auto result = SimulationRunner::runNonLinearWithHTC(mesh, ue, material, initialTemperature, config.timeStep, config.totalTime, ambientTemperature, boundaryConditions, config.picardTolerance, config.maxPicardIterations);

	int centerNodeIndex = mesh.findClosestNode(0.0, 0.025);

	ResultExporter::exportSummary(result, "validation/htc_model_check/variable_htc_contact_summary.csv", centerNodeIndex);

    return 0;
}