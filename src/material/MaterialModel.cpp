#include "MaterialModel.h"

#include<stdexcept>
#include<fstream>
#include <sstream>



MaterialModel::MaterialModel(double density, const std::vector<double>& temperatures, const std::vector<double>& conductivityValues, const std::vector<double>& specificHeatValues)
	:density(density), temperatures(temperatures), conductivityValues(conductivityValues), specificHeatValues(specificHeatValues)
{
	validateInput();
}

MaterialModel::MaterialModel(double density, const std::string& filePath) : density(density)
{
	loadFromCsv(filePath);
	validateInput();
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

void MaterialModel::validateInput() const
{
	if (this->temperatures.empty())
	{
		throw std::runtime_error("Temperature table cannot be empty.");
	}

	if (this->temperatures.size() != this->temperatures.size() || this->temperatures.size() != this->temperatures.size())
	{
		throw std::runtime_error("Material property tables must have the same size.");
	}

	if (this->temperatures.size() < 2)
	{
		throw std::runtime_error("At least two temperature points are required for interpolation.");
	}

	for (size_t i = 1; i < this->temperatures.size(); i++)
	{
		if (this->temperatures[i] <= this->temperatures[i - 1])
		{
			throw std::runtime_error("Temperature values in CSV must be strictly increasing");
		}
	}
}

void MaterialModel::loadFromCsv(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		throw std::runtime_error("Could not open material properties file: " + filePath);
	}

	std::string line;

	// Skip header
	if (!std::getline(file, line))
	{
		throw std::runtime_error("Material properties file is empty: " + filePath);
	}

	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		std::stringstream ss(line);
		std::string token;

		std::vector<double> rowValues;

		while (std::getline(ss, token, ','))
		{
			rowValues.push_back(std::stod(token));
		}

		if (rowValues.size() != 3)
		{
			throw std::runtime_error("Each row in material properties CSV must have exactly 3 values: temperature, conductivity, specific heat.");
		}

		this->temperatures.push_back(rowValues[0]);
		this->conductivityValues.push_back(rowValues[1]);
		this->specificHeatValues.push_back(rowValues[2]);
	}
}

