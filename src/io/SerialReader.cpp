#include "SerialReader.h"

#include <Windows.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>


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

	this->serialHandle = handle;

	std::this_thread::sleep_for(std::chrono::seconds(2));

	PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

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
			std::string line = readLine();

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

std::string SerialReader::readLine()
{
	if (!isOpen())
	{
		throw std::runtime_error("Serial port is not open.");
	}

	HANDLE handle = static_cast<HANDLE>(this->serialHandle);

	std::string line;

	char ch;
	DWORD bytesRead;

	while (true)
	{
		if (!ReadFile(handle, &ch, 1, &bytesRead, nullptr))
		{
			throw std::runtime_error("Failed to read from serial port.");
		}

		if (bytesRead == 0)
		{
			continue; // No data read, try again
		}

		if (ch == '\n')
		{
			break; // End of line
		}

		if (ch != '\r') // Ignore carriage return
		{
			line += ch;
		}
	}
	return line;
}
