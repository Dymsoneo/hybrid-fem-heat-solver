#pragma once


#include <string>

class ConsoleReporter
{
public:
    static void printHeader(const std::string& mode, int radialElements, int axialElements, const std::string& port = "", int baudRate = 0);

    static void printStepHeader();

    static void printStep(int step, double time, double furnaceTemperature, double centerTemperature, double realtimeFactor, bool deadlineMissed = false);

    static void printSummary(const std::string& status, int completedSteps, double simulationTime, double finalCenterTemperature, const std::string& resultsDirectory, int femSteps = -1);

    static void printAccurateStepHeader();

    static void printAccurateStep(int step, double time, double furnaceTemperature, double centerTemperature, int femSteps);;
};