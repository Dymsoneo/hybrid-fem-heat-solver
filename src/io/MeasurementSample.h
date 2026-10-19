#pragma once

#include <cstdint>

struct MeasurementSample
{
	std::uint64_t sampleId = 0;
	std::uint32_t arduinoTimeMs = 0;
	double pcTimeMs = 0.0;
	double temperatureC = 0.0;
};