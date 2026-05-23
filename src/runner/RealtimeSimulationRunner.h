#pragma once

#include <string>
#include <vector>

#include "../mesh/Mesh.h"
#include "../fem/UniversalElement.h"
#include "../material/MaterialModel.h"
#include "../boundary/BoundaryConditionManager.h"
#include "../config/SimulationConfig.h"


class RealtimeSimulationRunner
{
public:

	static void run(const std::string& protName, int baundRate, const std::string& outputFile, const Mesh& mesh, const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, const SimulationConfig& config,
		const std::vector<double>& initialTemperature, int centerNodeId, int maxSteps);

};