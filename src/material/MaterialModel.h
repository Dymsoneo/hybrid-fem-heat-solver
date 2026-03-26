#pragma once

#include <vector>

// Stores temperature-dependent material properties used in transient heat conduction simulations
class MaterialModel
{
	double density;

	std::vector<double> temperatures;
	std::vector<double> conductivityValues;
	std::vector<double> specificHeatValues;

	// Performs linear interpolation to find the property value at the given temperature
	double interpolate(double temperature, const std::vector<double>& values) const;

public:

	MaterialModel(double density, const std::vector<double>& temperatures, const std::vector<double>& conductivityValues, const std::vector<double>& specificHeatValues);

	double getDensity() const;

	double getConductivity(double temperature) const;
	double getSpecificHeat(double temperature) const;
};