#include "ConsoleReporter.h"

#include <iostream>
#include <iomanip>

void ConsoleReporter::printHeader(const std::string& mode, int radialElements, int axialElements, const std::string& port, int baudRate)
{
    std::cout << "\nHYBRID FEM HEAT SOLVER\n";
    std::cout << "====================================================\n";

    std::cout << "Mode: " << mode << '\n';
    std::cout << "Mesh: " << radialElements << " x " << axialElements << '\n';

    if (!port.empty())
    {
        std::cout << "Arduino: " << port << " @ " << baudRate << " baud\n";
    }

    std::cout << "====================================================\n\n";
}

void ConsoleReporter::printStepHeader()
{
    std::cout
        << std::right
        << std::setw(6) << "Step"
        << std::setw(12) << "Time [s]"
        << std::setw(16) << "Furnace [C]"
        << std::setw(15) << "Center [C]"
        << std::setw(12) << "RTF"
        << '\n';

    std::cout << std::string(61, '-') << '\n';
}

void ConsoleReporter::printStep(int step, double time, double furnaceTemperature, double centerTemperature, double realtimeFactor, bool deadlineMissed)
{
    // Save the original stream formatting.
    const auto oldFlags = std::cout.flags();
    const auto oldPrecision = std::cout.precision();

    std::cout
        << std::right
        << std::fixed
        << std::setprecision(2)
        << std::setw(6) << step
        << std::setw(12) << time
        << std::setw(16) << furnaceTemperature
        << std::setw(15) << centerTemperature
        << std::setw(11) << std::setprecision(1) << realtimeFactor
        << "x";

    if (deadlineMissed)
    {
        std::cout << "  [DEADLINE MISSED]";
    }

    std::cout << '\n';

    // Restore formatting for other console messages.
    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
}

void ConsoleReporter::printSummary(const std::string& status, int completedSteps, double simulationTime, double finalCenterTemperature, const std::string& resultsDirectory, int femSteps)
{
    const auto oldFlags = std::cout.flags();
    const auto oldPrecision = std::cout.precision();

    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "SESSION SUMMARY\n";
    std::cout << "====================================================\n";

    std::cout << std::left << std::fixed << std::setprecision(2);

    std::cout << std::setw(20) << "Status:" << status << '\n';

    std::cout << std::setw(20) << "Measurement steps:" << completedSteps << '\n';

    if (femSteps >= 0)
    {
        std::cout << std::setw(20) << "Total FEM steps:" << femSteps << '\n';
    }

    std::cout << std::setw(20) << "Simulation time:" << simulationTime << " s\n";

    std::cout << std::setw(20) << "Final center temp:" << finalCenterTemperature << " C\n";

    std::cout << std::setw(20) << "Results directory:" << resultsDirectory << '\n';

    std::cout << "====================================================\n";

    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
}


void ConsoleReporter::printAccurateStepHeader()
{
    std::cout << std::right << std::setw(6) << "Step" << std::setw(12) << "Time [s]" << std::setw(16) << "Furnace [C]" << std::setw(15) << "Center [C]" << std::setw(12) << "FEM steps" << '\n';

    std::cout << std::string(61, '-') << '\n';
}


void ConsoleReporter::printAccurateStep(int step, double time, double furnaceTemperature, double centerTemperature, int femSteps)
{
    const auto oldFlags = std::cout.flags();
    const auto oldPrecision = std::cout.precision();

    std::cout << std::right << std::fixed << std::setprecision(2) << std::setw(6) << step << std::setw(12) << time << std::setw(16) 
        << furnaceTemperature << std::setw(15) << centerTemperature << std::setw(12) << femSteps << '\n';

    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
}
