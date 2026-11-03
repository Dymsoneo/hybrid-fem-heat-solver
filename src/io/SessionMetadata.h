#pragma once

#include <filesystem>
#include <string>

#include "../config/AppConfig.h"

class SessionMetadata
{
public:
    static void writeInitial(const std::filesystem::path& sessionDir, const AppConfig& app, const std::string& configPath);

    static void writeFinal(const std::filesystem::path& sessionDir, const std::string& status, int completedSteps, double simulationTime, int femSteps = -1);
};
