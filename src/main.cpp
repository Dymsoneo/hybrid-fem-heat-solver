#include <iostream>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/SimulationRunner.h"
#include "io/ResultExporter.h"
#include "material/MaterialModel.h"
#include "fem/ElementMatrices.h"
#include "solver/TimeIntegrator.h"

int main()
{
   
	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 10, 20);
	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	std::vector<double> T(mesh.nodesCount, 20.0); // Initial temperature

	double dt = 10.0;
	double totalTime = 500.0;
	double alpha = 25.0;
	double ambientTemperature = 400.0;

	double tolerance = 1e-3;
	double maxIterations = 20;

	auto result = SimulationRunner::runNonLinear(mesh, ue, material, T, dt, totalTime, alpha, ambientTemperature, tolerance, maxIterations);

	ResultExporter::exportSummary(result, "results/simulation_data/nonlinear_summary.csv", mesh.findClosestNode(0, 0.025));

	

	return 0;

}