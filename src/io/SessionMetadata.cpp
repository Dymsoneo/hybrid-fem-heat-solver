#include "SessionMetadata.h"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <stdexcept>

void SessionMetadata::writeInitial(const std::filesystem::path& sessionDir, const AppConfig& app, const std::string& configPath)
{
    std::ofstream file(sessionDir / "session_info.txt");

    if (!file)
    {
        throw std::runtime_error("Cannot create session_info.txt");
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime =std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    file << std::setprecision(12);

    file << "mode=" << app.mode << '\n';
    file << "status=running\n";
    file << "start_time=" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << '\n';

    file << "config_file=" << configPath << '\n';

    file << "\n[mesh]\n";
    file << "radial_elements=" << app.simulation.radialElements << '\n';
    file << "axial_elements=" << app.simulation.axialElements << '\n';

    file << "\n[geometry]\n";
    file << "radius_m=" << app.radius << '\n';
    file << "height_m=" << app.height << '\n';

    file << "\n[material]\n";
    file << "density_kg_m3=" << app.density << '\n';
    file << "material_file=" << app.materialFile << '\n';

    file << "\n[boundary_conditions]\n";
    file << "htc_file=" << app.htcFile << '\n';

    file << "\n[solver]\n";
    file << "initial_temperature_c=" << app.initialTemperature << '\n';
    file << "picard_tolerance=" << app.simulation.picardTolerance << '\n';
    file << "max_picard_iterations=" << app.simulation.maxPicardIterations << '\n';

    if (app.mode == "realtime")
    {
        file << "\n[realtime]\n";
        file << "port=" << app.port << '\n';
        file << "baud_rate=" << app.baudRate << '\n';
        file << "max_steps=" << app.maxSteps << '\n';
        file << "measurement_time_source=arduino_millis\n";
    }
    else if (app.mode == "accurate")
    {
        file << "\n[accurate]\n";
        file << "measurement_file=" << app.measurementFile << '\n';
        file << "max_internal_time_step_s=" << app.accurateTimeStep << '\n';
        file << "furnace_interpolation=linear\n";
    }

    file.flush();

    if (!file)
    {
        throw std::runtime_error("Failed to write session_info.txt");
    }
}


void SessionMetadata::writeFinal(const std::filesystem::path& sessionDir, const std::string& status, int completedSteps, double simulationTime, int femSteps)
{
    const auto filePath = sessionDir / "session_info.txt";

    std::ifstream input(filePath);

    if (!input)
    {
        throw std::runtime_error("Cannot open session_info.txt");
    }

    std::string content;
    std::string line;

    while (std::getline(input, line))
    {
        if (line.rfind("status=", 0) == 0)
        {
            line = "status=" + status;
        }

        content += line + '\n';
    }

    input.close();

    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    std::ofstream output(filePath, std::ios::trunc);

    if (!output)
    {
        throw std::runtime_error("Cannot update session_info.txt");
    }

    output << content;
    output << "\nend_time=" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << '\n';
    output << "completed_steps=" << completedSteps << '\n';
    output << "simulation_time_s=" << simulationTime << '\n';

    if (femSteps >= 0)
    {
        output << "fem_steps=" << femSteps << '\n';
    }

    if (!output)
    {
        throw std::runtime_error(
            "Failed to update session_info.txt");
    }
}

