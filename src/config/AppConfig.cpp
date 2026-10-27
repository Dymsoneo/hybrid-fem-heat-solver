#include "AppConfig.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace
{
    std::string trim(const std::string& value)
    {
        const auto first = value.find_first_not_of(" \t\r\n");

        if (first == std::string::npos)
            return "";

        const auto last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    int parseInt(const std::string& value)
    {
        size_t pos = 0;
        int result = std::stoi(value, &pos);

        if (pos != value.size())
            throw std::invalid_argument("Invalid integer: " + value);

        return result;
    }

    double parseDouble(const std::string& value)
    {
        size_t pos = 0;
        double result = std::stod(value, &pos);

        if (pos != value.size() || !std::isfinite(result))
            throw std::invalid_argument("Invalid number: " + value);

        return result;
    }
}

AppConfig loadAppConfig(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
        throw std::runtime_error("Cannot open configuration file: " + filePath);

    std::unordered_map<std::string, std::string> values;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;
        line = trim(line);

        if (line.empty() || line[0] == '#')
            continue;

        const auto separator = line.find('=');

        if (separator == std::string::npos)
            throw std::runtime_error("Invalid configuration line: " + std::to_string(lineNumber));

        std::string key = trim(line.substr(0, separator));
        std::string value = trim(line.substr(separator + 1));

        if (key.empty() || value.empty())
            throw std::runtime_error("Empty configuration key or value at line " + std::to_string(lineNumber));

        if (!values.emplace(key, value).second)
            throw std::runtime_error("Duplicate configuration key: " + key);
    }

    auto get = [&](const std::string& key) -> std::string
        {
            auto it = values.find(key);

            if (it == values.end())
                throw std::runtime_error("Missing configuration parameter: " + key);

            return it->second;
        };

    const std::string mode = get("mode");


    AppConfig config;

    if (mode == "realtime")
        config.simulation = createRealtimeConfig();
    else if (mode == "accurate")
        config.simulation = createAccurateConfig();
    else
        throw std::runtime_error("Invalid simulation mode: " + mode);

    config.mode = mode;

    if (mode == "accurate")
    {
        config.measurementFile = get("measurement_file");

        config.accurateTimeStep =
            parseDouble(get("accurate_time_step"));

        if (config.accurateTimeStep <= 0.0)
        {
            throw std::runtime_error("accurate_time_step must be greater than zero.");
        }
    }

    try
    {
        config.port = get("port");
        config.baudRate = parseInt(get("baud_rate"));
        config.maxSteps = parseInt(get("max_steps"));

        config.simulation.radialElements = parseInt(get("radial_elements"));

        config.simulation.axialElements = parseInt(get("axial_elements"));

        config.simulation.picardTolerance = parseDouble(get("picard_tolerance"));

        config.simulation.maxPicardIterations = parseInt(get("max_picard_iterations"));

        config.radius = parseDouble(get("radius"));
        config.height = parseDouble(get("height"));
        config.density = parseDouble(get("density"));

        config.initialTemperature = parseDouble(get("initial_temperature"));

        config.materialFile = get("material_file");
        config.htcFile = get("htc_file");
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Invalid application configuration: " + std::string(e.what()));
    }

    if (config.baudRate <= 0 ||
        config.maxSteps <= 0 ||
        config.simulation.radialElements <= 0 ||
        config.simulation.axialElements <= 0 ||
        config.simulation.maxPicardIterations <= 0 ||
        config.simulation.picardTolerance <= 0.0 ||
        config.radius <= 0.0 ||
        config.height <= 0.0 ||
        config.density <= 0.0)
    {
        throw std::runtime_error("Configuration contains invalid non-positive parameters.");
    }

    return config;
}