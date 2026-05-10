#pragma once

#include <fstream>
#include <string>

class SimulationLogger
{
public:

    SimulationLogger() = default;

    explicit SimulationLogger(const std::string& filePath);

    ~SimulationLogger();



    void open(const std::string& filePath);

	void logStep(double timeSeconds, double furnanceTemperature, double centerSimulated, double minTemperature, double maxTemperature);

    void close();
    bool isOpen() const;

private:
    std::ofstream file;
    bool headerWritten{ false };

    void writeHeader();
};