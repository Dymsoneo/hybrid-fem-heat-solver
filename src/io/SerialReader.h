#pragma once

#include <string>

class SerialReader
{
public:
	SerialReader() = default;

	~SerialReader();

	void open(const std::string& portName, int baudRate);

	bool isOpen() const;

	double readTemperature();
	
	void close();

private:
	void* serialHandle = nullptr;

	std::string readLine();
};