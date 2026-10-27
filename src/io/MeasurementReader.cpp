#include "MeasurementReader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

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
        throw std::runtime_error("Failed to open measurement CSV file: " + filePath);

    std::string line;

    if (!std::getline(file, line))
        throw std::runtime_error("Measurement CSV file is empty.");

    // Remove Windows line ending if present
    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    const bool simpleFormat = line == "time_s,furnace_temperature_c";

    const bool acquisitionFormat = line == "sample_id,arduino_time_ms,pc_time_ms,temperature_c";

    if (!simpleFormat && !acquisitionFormat)
        throw std::runtime_error("Unsupported measurement CSV header.");

    samples.clear();
    currentIndex = 0;

    double firstArduinoTime = 0.0;
    double previousTime = -1.0;
    double previousSampleId = -1.0;
    bool firstSample = true;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<double> values;

        while (std::getline(ss, token, ','))
        {
            size_t pos = 0;
            double value = std::stod(token, &pos);

            if (pos != token.size() || !std::isfinite(value))
                throw std::runtime_error("Invalid CSV numeric value.");

            values.push_back(value);
        }

        if (values.size() != (simpleFormat ? 2u : 4u))
            throw std::runtime_error("Invalid CSV column count.");

        MeasurementSample sample;

        if (simpleFormat)
        {
            sample.timeSeconds = values[0];
            sample.furnaceTemperature = values[1];
        }
        else
        {
            const double sampleId = values[0];
            const double arduinoTime = values[1];

            if (sampleId < 0.0 || std::floor(sampleId) != sampleId || arduinoTime < 0.0 || std::floor(arduinoTime) != arduinoTime)
            {
                throw std::runtime_error("Invalid Arduino sample ID or timestamp.");
            }

            if (firstSample)
                firstArduinoTime = arduinoTime;
            else if (sampleId != previousSampleId + 1.0)
                throw std::runtime_error("Missing or out-of-order acquisition sample.");

            sample.timeSeconds =
                (arduinoTime - firstArduinoTime) / 1000.0;

            sample.furnaceTemperature = values[3];
            previousSampleId = sampleId;
        }

        if (!firstSample && sample.timeSeconds <= previousTime)
            throw std::runtime_error("Measurement timestamps must be strictly increasing.");

        previousTime = sample.timeSeconds;
        firstSample = false;

        samples.push_back(sample);
    }

    if (samples.empty())
        throw std::runtime_error("No measurement samples found.");
}

