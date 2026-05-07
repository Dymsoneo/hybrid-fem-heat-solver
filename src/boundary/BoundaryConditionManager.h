#pragma once

#include <map>

#include "BoundarySide.h"
#include "HTCModel.h"

// Class to manage boundary conditions for all sides of the domain. It allows setting and retrieving HTC models for each boundary side.

class BoundaryConditionManager
{
public:
	void setHTCModel(BoundarySide side, const HTCModel& model);

	bool hasHTCModel(BoundarySide side) const;

	const HTCModel& getHTCModel(BoundarySide side) const;

private:

	std::map<BoundarySide, HTCModel> htcModels;
};
