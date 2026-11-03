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

    double accurateTimeStep = 0.1;

    std::string materialFile =
        "data/materials/magnesium_alloy.csv";

    std::string bcRight = "table";
    std::string bcTop = "table";
    std::string bcBottom = "insulated";

    double htcRightValue = 100.0;
    double htcTopValue = 100.0;
    double htcBottomValue = 100.0;

    std::string htcRightFile = "data/boundary/htc_effective.csv";
    std::string htcTopFile = "data/boundary/htc_effective.csv";
    std::string htcBottomFile = "data/boundary/htc_effective.csv";

    std::string mode = "realtime";
    std::string measurementFile = "";
};

AppConfig loadAppConfig(const std::string& filePath);