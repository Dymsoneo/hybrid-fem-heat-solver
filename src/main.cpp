#include <iostream>

#include "mesh/MeshGenerator.h"
#include "fem/UniversalElement.h"
#include "fem/Assembly.h"
#include "solver/SimulationRunner.h"
#include "io/ResultExporter.h"

int main()
{
    Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, 10, 10);
    UniversalElement ue;

    double conductivity = 150.0;
    double density = 1800.0;
    double specificHeat = 1000.0;
    double alpha = 25.0;
    double ambientTemperature = 400.0;
    double dt = 10;

	double totalTime = 4500;

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

	auto result = SimulationRunner::runSimulation(system, T, dt, totalTime);

	std::cout << "Transient simulation results:\n\n";

    for (size_t step = 0; step < result.temperatureHistory.size(); step++)
    {
		const auto& temperatures = result.temperatureHistory[step];

		double minTemp = *std::min_element(temperatures.begin(), temperatures.end());
		double maxTemp = *std::max_element(temperatures.begin(), temperatures.end());

		std::cout << "Time: " << result.timePoints[step] << ", Min Temp: " << minTemp << ", Max Temp: " << maxTemp << "\n";
    }

	ResultExporter::exportTemperatureHistory(result, "results/simulation_data/temperature_history.csv");

	ResultExporter::exportSummary(result, "results/simulation_data/summary.csv", 0);

    return 0;
}