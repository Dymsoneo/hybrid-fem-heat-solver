#include "ExperimentLogger.h"

#include <iomanip>
#include <stdexcept>

ExperimentLogger::ExperimentLogger(const std::string& filePath, ExperimentLogFormat format)
{
	open(filePath, format);
}

ExperimentLogger::~ExperimentLogger()
{
	close();
}

void ExperimentLogger::open(const std::string& filePath, ExperimentLogFormat format)
{
	close();

	this->currentFormat = format;
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

	if (currentFormat != ExperimentLogFormat::Legacy)
	{
		throw std::runtime_error(
			"Invalid log format for logMeasurement.");
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

void ExperimentLogger::logSample(const MeasurementSample& sample)
{
	if (!this->file.is_open())
	{
		throw std::runtime_error("Log file is not open.");
	}

	if (currentFormat != ExperimentLogFormat::Acquisition)
	{
		throw std::runtime_error(
			"Invalid log format for logSample.");
	}

	this->file << sample.sampleId << ","
		<< sample.arduinoTimeMs << ","
		<< std::fixed << std::setprecision(3)
		<< sample.pcTimeMs << ","
		<< std::setprecision(2)
		<< sample.temperatureC << "\n";

	if (!this->file)
	{
		throw std::runtime_error(
			"Failed to write measurement sample.");
	}
}

void ExperimentLogger::writeHeader()
{
	if (this->headerWritten)
	{
		return;
	}

	if (currentFormat == ExperimentLogFormat::Acquisition)
	{
		file << "sample_id,arduino_time_ms,pc_time_ms,temperature_c\n";
	}
	else
	{
		file << "time_s,furnance_temperature_c\n";
	}

	this->headerWritten = true;
}
