#pragma once

#include <fstream>
#include <string>

class RealtimePerformanceLogger
{
public:
	RealtimePerformanceLogger() = default;

	explicit RealtimePerformanceLogger(const std::string& filePath);

	~RealtimePerformanceLogger();

	void open(const std::string& filePath);
	void logStep(int step, double simulationTimeSeconds, double computeTimeMs, int picardIterations, double realtimeSpeedup, bool deadlineMissed);
	void close();
	bool isOpen() const;

private:
	std::ofstream file;

	void writeHeader();
};
