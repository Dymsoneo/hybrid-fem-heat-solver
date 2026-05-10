#include "SimulationLogger.h"

#include <iomanip>
#include <stdexcept>

SimulationLogger::SimulationLogger(const std::string& filePath)
{
	open(filePath);
}

SimulationLogger::~SimulationLogger()
{
	close();
}

void SimulationLogger::open(const std::string& filePath)
{
	close();

	this->file.open(filePath);

	if (!this->file.is_open())
	{
		throw std::runtime_error("Failed to open log file: " + filePath);
	}

	headerWritten = false;
	writeHeader();
}

void SimulationLogger::logStep(double timeSeconds, double furnanceTemperature, double centerSimulated, double minTemperature, double maxTemperature)
{
	if (!this->file.is_open())
	{
		throw std::runtime_error("Log file is not open.");
	}

	this->file << std::fixed << std::setprecision(6) << timeSeconds << "," << furnanceTemperature << "," << centerSimulated << "," << minTemperature << "," << maxTemperature << std::endl;
}

void SimulationLogger::close()
{
	if (this->file.is_open())
	{
		this->file.close();
	}
}

bool SimulationLogger::isOpen() const
{
	return this->file.is_open();
}

void SimulationLogger::writeHeader()
{
	if(!this->headerWritten)
	{
		file << "time_s,furnance_temperature_c,center_simulated_c,t_min_c,t_max_c\n";
		headerWritten = true;
	}
}
