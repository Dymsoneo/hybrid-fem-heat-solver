#include "HTCModel.h"

#include<fstream>
#include <sstream>
#include <stdexcept>

HTCModel::HTCModel(double constantHTC): isConstant(true), constantHTC(constantHTC)
{
	if (constantHTC <= 0.0)
	{
		throw std::runtime_error("Constant HTC must be positive.");
	}
}

HTCModel::HTCModel(const std::vector<double>& surfaceTemperatures, const std::vector<double>& htcValues): isConstant(false), surfaceTemperatures(surfaceTemperatures), htcValues(htcValues)
{
	validateInput();
}

HTCModel::HTCModel(const std::string& filePath): isConstant(false)
{
	loadFromCSV(filePath);
	validateInput();
}



double HTCModel::getHTC(double surfaceTemperature) const
{
	if (this->isConstant)
	{
		return this->constantHTC;
	}
	return interpolate(surfaceTemperature);
}

double HTCModel::interpolate(double surfaceTemperature) const
{
    if (surfaceTemperature <= this->surfaceTemperatures.front())
    {
        return this->htcValues.front();
    }

    if (surfaceTemperature >= this->surfaceTemperatures.back())
    {
        return this->htcValues.back();
    }

    for (size_t i = 0; i < this->surfaceTemperatures.size() - 1; i++)
    {
        double T1 = this->surfaceTemperatures[i];
        double T2 = this->surfaceTemperatures[i + 1];

        if (surfaceTemperature >= T1 && surfaceTemperature <= T2)
        {
            double h1 = this->htcValues[i];
            double h2 = this->htcValues[i + 1];

            double factor = (surfaceTemperature - T1) / (T2 - T1);

            return h1 + factor * (h2 - h1);
        }
    }

    throw std::runtime_error("HTC interpolation failed.");
}

void HTCModel::loadFromCSV(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open HTC CSV file: " + filePath);
    }

    std::string line;

    // Skip header
    if (!std::getline(file, line))
    {
        throw std::runtime_error("HTC CSV file is empty: " + filePath);
    }

    while (std::getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string token;
        std::vector<double> values;

        while (std::getline(ss, token, ','))
        {
            values.push_back(std::stod(token));
        }

        if (values.size() != 2)
        {
            throw std::runtime_error(
                "Invalid HTC CSV row. Expected 2 columns: surface_temperature,htc."
            );
        }

        this->surfaceTemperatures.push_back(values[0]);
        this->htcValues.push_back(values[1]);
    }
}

void HTCModel::validateInput() const
{
    if (this->surfaceTemperatures.empty())
    {
        throw std::runtime_error("HTC temperature table cannot be empty.");
    }

    if (this->surfaceTemperatures.size() != this->htcValues.size())
    {
        throw std::runtime_error("HTC tables must have the same size.");
    }

    if (this->surfaceTemperatures.size() < 2)
    {
        throw std::runtime_error("At least two HTC data points are required.");
    }

    for (size_t i = 1; i < this->surfaceTemperatures.size(); i++)
    {
        if (this->surfaceTemperatures[i] <= this->surfaceTemperatures[i - 1])
        {
            throw std::runtime_error("HTC temperature values must be strictly increasing.");
        }
    }

    for (double h : this->htcValues)
    {
        if (h <= 0.0)
        {
            throw std::runtime_error("HTC values must be positive.");
        }
    }
}



