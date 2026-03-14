#include "Mesh.h"

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

void Mesh::clear() {
	nodes.clear();
	elements.clear();
	nodesCount = 0;
	elementsCount = 0;
}
