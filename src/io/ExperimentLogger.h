#pragma once

#include <fstream>
#include <string>

class ExperimentLogger
{
public:
	ExperimentLogger() = default;

	explicit ExperimentLogger(const std::string& filePath);

	~ExperimentLogger();

	
	void open(const std::string& filePath);

	void logMeasurement(double timeSeconds, double furnanceTemperature);

	void close();
	bool isOpen() const;

private:
	std::ofstream file;
	bool headerWritten{ false };

	void writeHeader();
};