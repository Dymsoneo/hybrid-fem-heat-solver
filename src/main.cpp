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
   
	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 5, 5);
	UniversalElement ue;

	MaterialModel material(1700.0, { 20.0, 100.0, 200.0, 300.0 }, { 160.0, 155.0, 148.0, 142.0 }, { 1000.0, 1030.0, 1080.0, 1120.0 });

	std::vector<double> T(mesh.nodesCount, 20.0); // Initial temperature

	double dt = 10.0;
	double alpha = 25.0;
	double ambientTemperature = 400.0;

	double tolerance = 1e-3;
	double maxIterations = 20;

	auto Tnext = TimeIntegrator::stepNonLinear(mesh, ue, material, T, dt, alpha, ambientTemperature, maxIterations, tolerance);

	for(double t : Tnext)
	{
		std::cout << t << std::endl;
	}

	return 0;

}