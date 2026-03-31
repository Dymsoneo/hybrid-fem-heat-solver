#include <iostream>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/SimulationRunner.h"
#include "io/ResultExporter.h"
#include "material/MaterialModel.h"

int main()
{
   
	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 5, 5);

	UniversalElement ue;

	MaterialModel material(1700.0, { 20.0, 100.0, 200.0, 300.0 }, { 160.0,155.0,148.0,142.0 }, { 1000.0,1030.0,1080.0,1120.0 });

	std::vector<double> nodalTemperatures(mesh.nodesCount, 20.0); // Initial temperature

	const Element& element = mesh.elements[0];

	auto H = ElementMatrices::computeHWithMaterialModel(element, mesh, ue, material, nodalTemperatures);

	auto C = ElementMatrices::computeCWithMaterialModel(element, mesh, ue, material, nodalTemperatures);

	std::cout << "H[0][0]: " << H[0][0] << std::endl;
	std::cout<< "C[0][0]: " << C[0][0] << std::endl;
	return 0;
}