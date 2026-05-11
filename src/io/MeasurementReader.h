#pragma once

#include <string>
#include <vector>

struct MeasurementSample
{
	double timeSeconds = 0.0;
	double furnaceTemperature = 0.0;
};

class MeasurementReader
{
public:

	MeasurementReader() = default;
	explicit MeasurementReader(const std::string& filePath);

	void load(const std::string& filePath);

	bool hasNext() const;

	MeasurementSample next();

	void reset();

	std::size_t size() const;

private:

	std::vector<MeasurementSample> samples;
	std::size_t currentIndex{ 0 };

	void loadFromCsv(const std::string& filePath);
};