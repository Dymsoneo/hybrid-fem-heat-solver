#include "MaterialModel.h"

#include<stdexcept>




MaterialModel::MaterialModel(double density, const std::vector<double>& temperatures, const std::vector<double>& conductivityValues, const std::vector<double>& specificHeatValues)
	:density(density), temperatures(temperatures), conductivityValues(conductivityValues), specificHeatValues(specificHeatValues)
{
	if (this->temperatures.empty())
	{
		throw std::runtime_error("Temperature table cannot be empty.");
	}

	if (this->temperatures.size() != this->conductivityValues.size() || this->temperatures.size() != this->specificHeatValues.size())
	{
		throw std::runtime_error("Material property tables must have the same size.");
	}

	if (temperatures.size() < 2)
	{
		throw std::runtime_error("At least two temperature points are required for interpolation.");	
	}

	
}

double MaterialModel::getDensity() const
{
	return this->density;
}

double MaterialModel::getConductivity(double temperature) const
{
	return interpolate(temperature, this->conductivityValues);
}

double MaterialModel::getSpecificHeat(double temperature) const
{
	return interpolate(temperature, this->specificHeatValues);
}


double MaterialModel::interpolate(double temperature, const std::vector<double>& values) const
{
	// Below range -> clamp to first value
	if(temperature <= this->temperatures.front())
	{
		return values.front();
	}

	// Above range -> clamp to last value
	if (temperature >= this->temperatures.back())
	{
		return values.back();
	}

	// Find interval [T_i, T_{i+1}]
	for (size_t i = 0; i < this->temperatures.size() - 1; i++)
	{
		double T1 = this->temperatures[i];
		double T2 = this->temperatures[i + 1];

		if (temperature >= T1 && temperature <= T2)
		{
			double v1 = values[i];
			double v2 = values[i + 1];

			double alpha = (temperature - T1) / (T2 - T1);
			return v1 + alpha * (v2 - v1);
		}
	}
	
	// Should never reach here due to clamping
	throw std::runtime_error("Interpolation error: temperature out of bounds.");
}

