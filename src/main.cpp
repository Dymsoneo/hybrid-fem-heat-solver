#include <iostream>
#include <exception>
#include <vector>
#include <filesystem>
#include <stdexcept>

#include "mesh/MeshGenerator.h"
#include "config/AppConfig.h"
#include "runner/RealtimeSimulationRunner.h"
#include "io/SessionMetadata.h"
#include "runner/ReplaySimulationRunner.h"
#include "io/SessionManager.h"

int main(int argc, char* argv[])
{
    try
    {
        

        // Configuration file path
        std::string configPath = "config/accurate.cfg";

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
            RealtimeSimulationRunner::run(app.port, app.baudRate,
                mesh, ue, material, boundaryConditions, config, initialTemperature, centerNodeId, app.maxSteps, app, configPath);
        }
        else if (app.mode == "accurate")
        {

            if (!std::filesystem::is_regular_file(app.measurementFile))
            {
                throw std::runtime_error("Measurement file not found: " + app.measurementFile);
            }

            const auto sessionDir = SessionManager::createSessionDirectory();

            SessionMetadata::writeInitial(sessionDir, app, configPath);

            const auto outputFile = (sessionDir / "simulation.csv").string();

            std::cout << "Accurate session directory: " << sessionDir.string() << '\n';


            ReplaySummary summary;

            try
            {
                ReplaySimulationRunner::run(app.measurementFile, outputFile, mesh, ue, material, 
                    boundaryConditions, config, initialTemperature, centerNodeId, app.accurateTimeStep, summary);

                SessionMetadata::writeFinal(sessionDir, "completed", summary.completedSteps, summary.simulationTime, summary.femSteps);
            }
            catch (const std::exception& e)
            {
                try
                {
                    SessionMetadata::writeFinal(sessionDir, "failed", summary.completedSteps, summary.simulationTime, summary.femSteps);
                }
                catch (const std::exception& metadataError)
                {
                    std::cerr << "Warning: Could not update session metadata: " << metadataError.what() << '\n';
                }

                throw;
            }

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
