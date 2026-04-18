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
	
	for (int i = 0; i < 4; i++) 
	{

		int meshX, meshY;
		switch (i)
		{
		case 0:
			meshX = 5;
			meshY = 10;
			break;
		case 1:
			meshX = 10;
			meshY = 20;
			break;
		case 2:
			meshX = 20;
			meshY = 40;
			break;
		default:
			meshX = 30;
			meshY = 60;
			break;
		}

		Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, meshX, meshY);
		UniversalElement ue;

		std::cout << "Mesh: " << mesh.nodesCount << " nodes, " << mesh.elementsCount << " elements." << std::endl;

		double dt = 10.0;
		double totalTime = 500.0;

		double alpha = 25.0;

		double density = 1700.0;

		double ambientTemperature = 400.0;

		std::vector<double> T(mesh.nodesCount, 20.0); // Initial temperature

		// Run nonlinear simulation

		MaterialModel material(density, "data/materials/magnesium_alloy.csv");

		double tolerance = 1e-3;
		double maxIterations = 20;

		auto result = SimulationRunner::runNonLinear(mesh, ue, material, T, dt, totalTime, alpha, ambientTemperature, tolerance, maxIterations);

		std::cout << "T center: " << result.temperatureHistory.back()[mesh.findClosestNode(0.0, 0.025)] << std::endl;
		std::cout << "T max: " << *std::max_element(result.temperatureHistory.back().begin(), result.temperatureHistory.back().end()) << std::endl;

	}
	
	return 0;
}