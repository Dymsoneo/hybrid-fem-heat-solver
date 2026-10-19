#pragma once

#include <string>
#include <chrono>

#include "MeasurementSample.h"

class SerialReader
{
public:
	SerialReader() = default;

	~SerialReader();

	void open(const std::string& portName, int baudRate);

	bool isOpen() const;

	double readTemperature();
	
	void close();

	MeasurementSample readSample();

private:
	void* serialHandle = nullptr;

	std::string readLine();

	std::chrono::steady_clock::time_point acquisitionStart;
};