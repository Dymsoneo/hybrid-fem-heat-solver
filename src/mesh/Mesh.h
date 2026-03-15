#pragma once

#include <vector>

#include "Element.h"
#include "Node.h"

// Representation of a mesh consisting of nodes and elements
class Mesh {
public:
    std::vector<Node> nodes;
    std::vector<Element> elements;

    int nodesCount;
    int elementsCount;

    Mesh();

    void addNode(const Node& node);
    void addElement(const Element& element);

    void clear();
};
