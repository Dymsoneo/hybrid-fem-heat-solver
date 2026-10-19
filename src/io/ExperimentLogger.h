#pragma once

#include <fstream>
#include <string>

#include "MeasurementSample.h"

enum class ExperimentLogFormat
{
	Legacy,
	Acquisition
};

class ExperimentLogger
{
public:
	ExperimentLogger() = default;

	explicit ExperimentLogger(const std::string& filePath, ExperimentLogFormat format);

	~ExperimentLogger();

	
	void open(const std::string& filePath, ExperimentLogFormat format);

	void logMeasurement(double timeSeconds, double furnanceTemperature);

	void close();
	bool isOpen() const;

	void logSample(const MeasurementSample& sample);

private:
	std::ofstream file;
	bool headerWritten{ false };

	ExperimentLogFormat currentFormat{
		ExperimentLogFormat::Legacy
	};

	void writeHeader();
};