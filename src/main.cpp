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
	// Parameters for both linear and nonlinear simulations

	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 20, 40);
	UniversalElement ue;

	double dt = 10.0;
	double totalTime = 500.0;

	double alpha = 25.0;

	double density = 1700.0;

	double ambientTemperature = 400.0;

	std::vector<double> T(mesh.nodesCount, 20.0); // Initial temperature

	// Run linear simulation

	std::cout << "Starting linear simulation..." << std::endl;

	double linearConductivity = 160;
	double linearSpecificHeat = 1000;

	auto system = Assembly::assembleSystem(mesh, ue, linearConductivity, density, linearSpecificHeat, alpha, ambientTemperature);

	auto result = SimulationRunner::runLinear(system, T, dt, totalTime);

	ResultExporter::exportSummary(result, "results/simulation_data/linear_summary.csv", mesh.findClosestNode(0, 0.025));

	std::cout << "Linear simulation completed. Starting nonlinear simulation..." << std::endl;

	// Run nonlinear simulation

	MaterialModel material(density, "data/materials/magnesium_alloy.csv");

	double tolerance = 1e-3;
	double maxIterations = 20;

	result = SimulationRunner::runNonLinear(mesh, ue, material, T, dt, totalTime, alpha, ambientTemperature, tolerance, maxIterations);

	ResultExporter::exportSummary(result, "results/simulation_data/nonlinear_summary.csv", mesh.findClosestNode(0, 0.025));
	
	std::cout << "Nonlinear simulation completed." << std::endl;
	
	return 0;

}