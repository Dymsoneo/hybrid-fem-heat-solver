
#include <iostream>
#include <exception>
#include <vector>

#include "mesh/MeshGenerator.h"
#include "config/AppConfig.h"
#include "runner/RealtimeSimulationRunner.h"
#include "io/MeasurementReader.h"
#include "runner/ReplaySimulationRunner.h"

int main(int argc, char* argv[])
{
    try
    {
        

        // Configuration file path
        std::string configPath = "config/realtime.cfg";

        if (argc == 3 && std::string(argv[1]) == "--config")
        {
            configPath = argv[2];
        }
        else if (argc != 1)
        {
            throw std::runtime_error("Usage: HybridFEMHeatSolver.exe --config <file.cfg>");
        }

        AppConfig app = loadAppConfig(configPath);
        SimulationConfig& config = app.simulation;

        std::cout << "Configuration loaded: " << configPath << '\n';

        // Mesh
        Mesh mesh = MeshGenerator::generateCylinderMesh(app.radius, app.height, config.radialElements, config.axialElements);

        UniversalElement ue;

        // Material
        MaterialModel material(app.density, app.materialFile);

        // Boundary conditions
        BoundaryConditionManager boundaryConditions;

        HTCModel furnaceHTC(app.htcFile);

        boundaryConditions.setHTCModel(BoundarySide::Right, furnaceHTC);

        boundaryConditions.setHTCModel(BoundarySide::Top, furnaceHTC);

        // Initial condition
        std::vector<double> initialTemperature(mesh.nodes.size(), app.initialTemperature);

        int centerNodeId = mesh.findClosestNode(0.0, app.height / 2.0);

        // Realtime simulation
        if (app.mode == "realtime")
        {
            RealtimeSimulationRunner::run(app.port, app.baudRate, "unused.csv", "unused_performance.csv", 
                mesh, ue, material, boundaryConditions, config, initialTemperature, centerNodeId, app.maxSteps);
        }
        else if (app.mode == "accurate")
        {
            ReplaySimulationRunner::run(app.measurementFile, "results/accurate_test.csv", 
                mesh, ue, material, boundaryConditions, config, initialTemperature, centerNodeId);
        }

        std::cout << "Simulation completed.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
