#pragma once

#include <string>
#include <vector>

#include "../mesh/Mesh.h"
#include "../fem/UniversalElement.h"
#include "../material/MaterialModel.h"
#include "../boundary/BoundaryConditionManager.h"
#include "../config/SimulationConfig.h"
#include "../config/AppConfig.h"
#include "../io/ConsoleReporter.h"


class RealtimeSimulationRunner
{
public:

	static void run(const std::string& portName, int baudRate,  const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, const SimulationConfig& config,
		const std::vector<double>& initialTemperature, int centerNodeId, int maxSteps, const AppConfig& app, const std::string& configPath);

};