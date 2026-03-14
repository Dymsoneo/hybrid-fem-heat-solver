#pragma once
#include "Mesh.h"

class MeshGenerator {
public:
	static Mesh generateCylinderMesh(double radius, double height, int elementsR, int elementsZ);
};
