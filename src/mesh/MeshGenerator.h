#pragma once
#include "Mesh.h"

// Mesh generator for axisymmetric problems using Q4 finite elements
class MeshGenerator {
public:
	static Mesh generateCylinderMesh(double radius, double height, int elementsR, int elementsZ);
};
