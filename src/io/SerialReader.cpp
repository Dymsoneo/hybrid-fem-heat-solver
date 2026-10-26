#include "SerialReader.h"

#include <Windows.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>


SerialReader::~SerialReader()
{
	close();
}

void SerialReader::open(const std::string& portName, int baudRate)
{
	HANDLE handle = CreateFileA(portName.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);


	if (handle == INVALID_HANDLE_VALUE)
	{
		throw std::runtime_error("Failed to open serial port.");
	}

	DCB dcbSerialParams = { 0 };

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(handle, &dcbSerialParams))
	{
		CloseHandle(handle);
		throw std::runtime_error("Failed to get serial port state.");
	}

	dcbSerialParams.BaudRate = baudRate;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(handle, &dcbSerialParams))
	{
		CloseHandle(handle);
		throw std::runtime_error("Failed to set serial port state.");
	}

	COMMTIMEOUTS timeouts = {};
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.ReadTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(handle, &timeouts))
	{
		CloseHandle(handle);
		throw std::runtime_error("Failed to set serial port timeouts.");
	}

	this->serialHandle = handle;

	std::this_thread::sleep_for(std::chrono::seconds(2));

	PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

	acquisitionStart = std::chrono::steady_clock::now();
}

bool SerialReader::isOpen() const
{
	return this->serialHandle != nullptr;
}

double SerialReader::readTemperature()
{
	while (true)
	{
		try
		{
			const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
			const std::string line = readLine(deadline);

			if (line.empty())
			{
				continue;
			}

			return std::stod(line);

		}
		catch (const std::exception& e)
		{
			continue;
		}
	}
}

void SerialReader::close()
{
	if (this->serialHandle != nullptr)
	{
		CloseHandle(static_cast<HANDLE>(this->serialHandle));


		this->serialHandle = nullptr;
	}
}

std::string SerialReader::readLine(std::chrono::steady_clock::time_point deadline)
{
	if (!isOpen())
	{
		throw std::runtime_error("Serial port is not open.");
	}

	HANDLE handle = static_cast<HANDLE>(serialHandle);

	std::string line;
	char ch;
	DWORD bytesRead;

	while (std::chrono::steady_clock::now() < deadline)
	{
		if (!ReadFile(handle, &ch, 1, &bytesRead, nullptr))
		{
			throw std::runtime_error("Failed to read from serial port.");
		}

		if (bytesRead == 0)
		{
			continue;
		}

		if (ch == '\n')
		{
			return line;
		}

		if (ch != '\r')
		{
			line += ch;
		}

		if (line.size() > 256)
		{
			throw std::runtime_error("Serial frame exceeds maximum length.");
		}
	}

	throw std::runtime_error("Serial read timeout: no complete frame received.");
}

MeasurementSample SerialReader::readSample()
{
	while (true)
	{
		const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
		const std::string line = readLine(deadline);
		const auto receivedAt = std::chrono::steady_clock::now();

		std::stringstream ss(line);
		std::string id, timestamp, temperature, extra;

		if (!std::getline(ss, id, ',') ||
			!std::getline(ss, timestamp, ',') ||
			!std::getline(ss, temperature, ',') ||
			std::getline(ss, extra, ','))
		{
			continue;
		}

		try
		{
			size_t p1 = 0, p2 = 0, p3 = 0;

			const auto parsedId = std::stoull(id, &p1);
			const auto parsedTime = std::stoul(timestamp, &p2);
			const auto parsedTemp = std::stod(temperature, &p3);

			if (p1 != id.size() ||
				p2 != timestamp.size() ||
				p3 != temperature.size() ||
				parsedTime > UINT32_MAX ||
				!std::isfinite(parsedTemp))
			{
				continue;
			}

			MeasurementSample sample;
			sample.sampleId = parsedId;
			sample.arduinoTimeMs =
				static_cast<std::uint32_t>(parsedTime);
			sample.temperatureC = parsedTemp;

			sample.pcTimeMs =
				std::chrono::duration<double, std::milli>(
					receivedAt - acquisitionStart).count();

			return sample;
		}
		catch (const std::exception&)
		{
			continue;
		}
	}
}

void SerialReader::clearInputBuffer()
{
	if (!isOpen())
	{
		throw std::runtime_error("Serial port is not open.");
	}
	
	HANDLE handle = static_cast<HANDLE>(serialHandle);
	
	
	if (!PurgeComm(handle, PURGE_RXCLEAR))
	{
		throw std::runtime_error("Failed to clear serial port input buffer.");
	}
}
