#include "Mesh.h"

#include<cmath>

Mesh::Mesh() : nodesCount(0), elementsCount(0)
{
}

void Mesh::addNode(const Node& node)
{
	nodes.push_back(node);
	nodesCount = nodes.size();
}

void Mesh::addElement(const Element& element)
{
	elements.push_back(element);
	elementsCount = elements.size();
}

int Mesh::findClosestNode(double r, double z)
{
	double minDistance = 1e9;
	int bestIndex = 0;

	for (int i = 0; i < nodesCount; i++)
	{
		double dr = nodes[i].r - r;
		double dz = nodes[i].z - z;
		double distance = std::sqrt(dr * dr + dz * dz);
		if(distance < minDistance)
		{
			minDistance = distance;
			bestIndex = i;
		}
	}


	return bestIndex;
}

void Mesh::clear() {
	nodes.clear();
	elements.clear();
	nodesCount = 0;
	elementsCount = 0;
}
