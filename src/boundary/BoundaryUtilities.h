#pragma once

#include <stdexcept>

#include "BoundarySide.h"

// Maps local Q4 edge index to physical boundary side

// edge 0 -> Bottom
// edge 1 -> Right
// edge 2 -> Top
// edge 3 -> Left

inline BoundarySide getBoundarySideFromEdge(int edgeIndex)
{
	switch (edgeIndex)
	{
	case 0:
		return BoundarySide::Bottom;
	case 1:
		return BoundarySide::Right;
	case 2:
		return BoundarySide::Top;
	case 3:
		return BoundarySide::Left;
	default:
		throw std::invalid_argument("Invalid edge index for Q4 element.");
	}
}