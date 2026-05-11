#include "MeasurementReader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

MeasurementReader::MeasurementReader(const std::string& filePath)
{
	load(filePath);
}

void MeasurementReader::load(const std::string& filePath)
{
	this->samples.clear();

	this->currentIndex = 0;

	loadFromCsv(filePath);
}

bool MeasurementReader::hasNext() const
{
	return this->currentIndex < this->samples.size();
}

MeasurementSample MeasurementReader::next()
{
	if (!hasNext())
	{
		throw std::runtime_error("No more measurement samples available.");
	}

	return this->samples[this->currentIndex++];	
}

void MeasurementReader::reset()
{
	this->currentIndex = 0;
}

std::size_t MeasurementReader::size() const
{
	return this->samples.size();
}

void MeasurementReader::loadFromCsv(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open measurement CSV file: " + filePath);
    }

    std::string line;

    // Skip header
    if (!std::getline(file, line))
    {
        throw std::runtime_error("Measurement CSV file is empty: " + filePath);
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
                "Invalid measurement CSV row. Expected 2 columns: time_s,furnace_temperature_c."
            );
        }

        MeasurementSample sample;
        sample.timeSeconds = values[0];
        sample.furnaceTemperature = values[1];

        this->samples.push_back(sample);
    }
}
