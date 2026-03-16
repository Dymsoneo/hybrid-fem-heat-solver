#include <iostream>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/TimerIntegrator.h"

int main()
{
    Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 5, 5);
    UniversalElement ue;

    double conductivity = 150.0;
    double density = 1800.0;
    double specificHeat = 1000.0;
    double alpha = 25.0;
    double ambientTemperature = 400.0;
    double dt = 1.0;

    auto system = Assembly::assembleSystem(
        mesh,
        ue,
        conductivity,
        density,
        specificHeat,
        alpha,
        ambientTemperature
    );

    std::vector<double> T(mesh.nodesCount, 20.0);

    auto Tnext = TimeIntegrator::step(system, T, dt);

    std::cout << "Temps after one time step:\n";
    int i = 1;
    for (double t : Tnext)
    {
        std::cout << "Node:\t" << i++ << "\t" << t << "\n";
    }

    return 0;
}