#include <iostream>
#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"

int main()
{
    Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 2, 2);
    UniversalElement ue;

    double conductivity = 150.0;
    double density = 1800.0;
    double specificHeat = 1000.0;
    double alpha = 25.0;
    double ambientTemperature = 400.0;

    auto system = Assembly::assembleSystem(
        mesh,
        ue,
        conductivity,
        density,
        specificHeat,
        alpha,
        ambientTemperature
    );

    std::cout << "Global H size: " << system.H.size() << " x " << system.H[0].size() << '\n';
    std::cout << "Global C size: " << system.C.size() << " x " << system.C[0].size() << '\n';
    std::cout << "Global Hbc size: " << system.Hbc.size() << " x " << system.Hbc[0].size() << '\n';
    std::cout << "Global P size: " << system.P.size() << '\n';

    std::cout << "\nFirst row of global P:\n";
    for (double value : system.P)
    {
        std::cout << value << " ";
    }
    std::cout << '\n';

    return 0;
}