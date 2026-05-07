#pragma once

enum class BoundarySide 
{
	Left,			// r = 0 (axis of symmetry)
	Right,			// r = R (outer surface)	
	Bottom,			// z = 0 (bottom surface)
	Top,			// z = H (top surface)
};