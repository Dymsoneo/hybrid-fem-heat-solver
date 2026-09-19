#include "RealtimePerformanceLogger.h"

#include <iomanip>
#include <stdexcept>

RealtimePerformanceLogger::RealtimePerformanceLogger(const std::string& filePath)
{
	open(filePath);
}

RealtimePerformanceLogger::~RealtimePerformanceLogger()
{
	close();
}

void RealtimePerformanceLogger::open(const std::string& filePath)
{
	close();
	
	this->file.open(filePath);

	if (!this->file.is_open())
	{
		throw std::runtime_error("Failed to open performance log file: " + filePath);
	}

	writeHeader();
}

void RealtimePerformanceLogger::logStep(int step, double simulationTimeSeconds, double computeTimeMs, int picardIterations, double realtimeSpeedup, bool deadlineMissed)
{
	if (!this->file.is_open())
	{
		throw std::runtime_error("Performance log file is not open.");
	}

	this->file << std::fixed << std::setprecision(6) << step << "," << simulationTimeSeconds << "," << computeTimeMs << "," << picardIterations << "," << realtimeSpeedup << "," << (deadlineMissed ? 1 : 0) << "\n";
}

void RealtimePerformanceLogger::close()
{
	if (this->file.is_open())
	{
		this->file.close();
	}
}

bool RealtimePerformanceLogger::isOpen() const
{
	return this->file.is_open();
}

void RealtimePerformanceLogger::writeHeader()
{
	this->file << "step,simulation_time_s,compute_time_ms,picard_iterations,realtime_speedup,deadline_missed\n";
}

