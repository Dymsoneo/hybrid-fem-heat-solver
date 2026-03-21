#include "ResultExporter.h"

#include <fstream>
#include <algorithm>
#include<stdexcept>


void ResultExporter::exportTemperatureHistory(const SimulationRunner::SimulationResult& result, const std::string& filePath)
{
	std::ofstream file(filePath);

	if(!file.is_open())
	{
		throw std::runtime_error("Could not open file for writing: " + filePath);
	}

	// CSV format: time, node_1, node_2, ..., node_n

	if (result.temperatureHistory.empty())
	{
		throw std::runtime_error("Temperature history is empty, cannot export.");
	}

	int nodesCount = static_cast<int>(result.temperatureHistory[0].size());

	file << "time";

	for (int i = 0; i < nodesCount; i++)
	{
		file << ",node_" << i;
	}
	file << "\n";

	for (size_t step = 0; step < result.temperatureHistory.size(); step++)
	{
		file << result.timePoints[step];
		for (double temperature : result.temperatureHistory[step])
		{
			file << "," << temperature;
		}
		file << "\n";
	}
}

void ResultExporter::exportSummary(const SimulationRunner::SimulationResult& result, const std::string& filePath, int selectedNodeIndex)
{
	std::ofstream file(filePath);

	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file for writing: " + filePath);
	}

	//CSV format: time, t_min, t_max, selected_node_temperature

	file << "time,t_min,t_max,selected_node_temperature\n";

	for (size_t step = 0; step < result.temperatureHistory.size(); step++)
	{
		const auto& temperatures = result.temperatureHistory[step];
		
		double tMin = *std::min_element(temperatures.begin(), temperatures.end());
		double tMax = *std::max_element(temperatures.begin(), temperatures.end());

		double selectedTemperature = 0.0;

		if (selectedNodeIndex >= 0 && selectedNodeIndex < static_cast<int>(temperatures.size()))
		{
			selectedTemperature = temperatures[selectedNodeIndex];
		}
		
		file << result.timePoints[step] << "," << tMin << "," << tMax << "," << selectedTemperature << "\n";
	}
}
