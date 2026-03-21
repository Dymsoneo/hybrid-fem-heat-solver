#pragma once

#include<string>

#include "../solver/SimulationRunner.h"

// Exports simulation results to CSV files for further analysis and visualization.
class ResultExporter
{
public:

	static void exportTemperatureHistory(const SimulationRunner::SimulationResult& result, const std::string& filePath);

	static void exportSummary(const SimulationRunner::SimulationResult& result, const std::string& filePath, int selectedNodeIndex);
};