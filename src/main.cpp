#include <iostream>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/SimulationRunner.h"
#include "io/ResultExporter.h"
#include "material/MaterialModel.h"

int main()
{
   
	MaterialModel material(7800.0, { 20.0, 100.0, 200.0 }, { 50.0, 45.0, 40.0 }, { 500.0, 480.0, 460.0 });

	double T = 150.0; 

	std::cout << "Temperature: " << T << "C\n";
	std::cout << "Density: " << material.getDensity() << " kg/m^3\n";
	std::cout << "k(T): " << material.getConductivity(T) << " W/mK\n";
	std::cout << "c(T): " << material.getSpecificHeat(T) << " J/kgK\n";

    return 0;
}