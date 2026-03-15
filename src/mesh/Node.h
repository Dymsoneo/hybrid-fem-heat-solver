#pragma once

// Representation of a single Node in axisymmetric model
class Node {
public:
	double r{}, z{};
	int id{};
	bool isBoundary{};
	
	Node() = default;

	Node(int id, double r, double z, bool isBoundary = false) : id(id), r(r), z(z), isBoundary(isBoundary) {  }
};