#pragma once

#include <string>

#include "SimulationConfig.h"

struct AppConfig
{
    SimulationConfig simulation;

    std::string port = "COM7";
    int baudRate = 9600;
    int maxSteps = 50;

    double radius = 0.02;
    double height = 0.05;
    double density = 1700.0;
    double initialTemperature = 20.0;

    std::string materialFile =
        "data/materials/magnesium_alloy.csv";

    std::string htcFile =
        "data/boundary/htc_effective.csv";

    std::string mode = "realtime";
    std::string measurementFile = "";
};

AppConfig loadAppConfig(const std::string& filePath);