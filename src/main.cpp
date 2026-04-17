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


int main()
{
	// Parameters for both linear and nonlinear simulations
	for (int i = 0; i < 3; i++) // Three runs to get average time, since some runs can be outliers due to various factors (e.g. CPU load)
	{

		Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 10, 20);
		UniversalElement ue;

		std::cout << "Mesh: " << mesh.nodesCount << " nodes, " << mesh.elementsCount << " elements." << std::endl;

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

		auto start = std::chrono::high_resolution_clock::now();

		auto system = Assembly::assembleSystem(mesh, ue, linearConductivity, density, linearSpecificHeat, alpha, ambientTemperature);

		auto result = SimulationRunner::runLinear(system, T, dt, totalTime);

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsedLinear = end - start;

		//ResultExporter::exportSummary(result, "results/simulation_data/linear_summary.csv", mesh.findClosestNode(0, 0.025));

		std::cout << "Linear simulation completed in " << elapsedLinear.count() << " seconds. " << elapsedLinear.count() / result.timePoints.size() << " seconds per time step." << std::endl;

		std::cout << "Linear simulation completed. Starting nonlinear simulation..." << std::endl;

		// Run nonlinear simulation

		MaterialModel material(density, "data/materials/magnesium_alloy.csv");

		double tolerance = 1e-3;
		double maxIterations = 20;

		start = std::chrono::high_resolution_clock::now();

		result = SimulationRunner::runNonLinear(mesh, ue, material, T, dt, totalTime, alpha, ambientTemperature, tolerance, maxIterations);

		end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsedNonLinear = end - start;

		//ResultExporter::exportSummary(result, "results/simulation_data/nonlinear_summary.csv", mesh.findClosestNode(0, 0.025));

		std::cout << "Nonlinear simulation completed in " << elapsedNonLinear.count() << " seconds. " << elapsedNonLinear.count() / result.timePoints.size() << " seconds per time step." << std::endl;
		std::cout << "Average Picard iterations per time step: "
			<< std::accumulate(result.picardIterations.begin(), result.picardIterations.end(), 0) / static_cast<double>(result.picardIterations.size()) << std::endl;
		std::cout << "Max Picard iterations in any time step: " << *std::max_element(result.picardIterations.begin(), result.picardIterations.end()) << std::endl;

		std::cout << "Nonlinear simulation completed." << std::endl;
	}
	
	return 0;
}