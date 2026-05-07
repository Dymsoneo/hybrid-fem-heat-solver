#include "BoundaryConditionManager.h"

#include <stdexcept>

void BoundaryConditionManager::setHTCModel(BoundarySide side, const HTCModel& model)
{
	this->htcModels[side] = model;
}

bool BoundaryConditionManager::hasHTCModel(BoundarySide side) const
{
	return this->htcModels.find(side) != this->htcModels.end();
}

const HTCModel& BoundaryConditionManager::getHTCModel(BoundarySide side) const
{
	auto it = this->htcModels.find(side);

	if (it == this->htcModels.end())
	{
		throw std::runtime_error("No HTC model defined for the specified boundary side.");
	}

	return it->second;
}
