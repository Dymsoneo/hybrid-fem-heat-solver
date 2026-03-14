#pragma once
#include<vector>
#include<array>
class Element {
public:
	int id;

	std::array<int, 4> nodeIds;

	// Local matrices 
	std::vector<std::vector<double>> H;
	std::vector<std::vector<double>> C;

	std::vector<double> P;

	Element() : H(4, std::vector<double>(4, 0.0)), C(4, std::vector<double>(4, 0.0)), P(4, 0.0), id(0), nodeIds{ 0,0,0,0 } {  }
	Element(int id, const std::array<int,4>& nodeIds) : id(id), nodeIds(nodeIds), H(4, std::vector<double>(4, 0.0)), C(4, std::vector<double>(4, 0.0)), P(4, 0.0) {  }
		

	
};