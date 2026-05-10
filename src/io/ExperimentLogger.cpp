#include "ExperimentLogger.h"

#include <iomanip>
#include <stdexcept>

ExperimentLogger::ExperimentLogger(const std::string& filePath)
{
	open(filePath);
}

ExperimentLogger::~ExperimentLogger()
{
	close();
}

void ExperimentLogger::open(const std::string& filePath)
{
	close();

	this->file.open(filePath);

	if (!this->file.is_open())
	{
		throw std::runtime_error("Failed to open log file: " + filePath);
	}

	this->headerWritten = false;
	writeHeader();
}

void ExperimentLogger::logMeasurement(double timeSeconds, double furnanceTemperature)
{
	if (!this->file.is_open())
	{
		throw std::runtime_error("Log file is not open.");
	}

	this->file << std::fixed << std::setprecision(6) << timeSeconds << "," << furnanceTemperature << std::endl;
}

void ExperimentLogger::close()
{
	if (this->file.is_open())
	{
		this->file.close();
	}
}

bool ExperimentLogger::isOpen() const
{
	return this->file.is_open();
}

void ExperimentLogger::writeHeader()
{
	if (!this->headerWritten)
	{
		file << "time_s,furnance_temperature_c\n";
		headerWritten = true;
	}
}
