#include "RealtimeSimulationRunner.h"

#include <chrono>

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <conio.h>

#include "../io/SerialReader.h"
#include "../io/SimulationLogger.h"
#include "../solver/TimeIntegrator.h"
#include "../io/RealtimePerformanceLogger.h"
#include "../io/ExperimentLogger.h"

void RealtimeSimulationRunner::run(const std::string& portName, int baudRate, const std::string& outputFile, const std::string& performanceOutputFile, const Mesh& mesh,
	const UniversalElement& ue, const MaterialModel& material, const BoundaryConditionManager& boundaryConditions, const SimulationConfig& config, const std::vector<double>& initialTemperature, int centerNodeId, int maxSteps)
{
    if (maxSteps <= 0)
    {
        throw std::runtime_error("Max steps must be greater than 0");
    }

    if (centerNodeId < 0 ||
        static_cast<size_t>(centerNodeId) >= initialTemperature.size())
    {
        throw std::runtime_error("Invalid center node ID");
    }

    SerialReader serialReader;
    serialReader.open(portName, baudRate);

    std::cout << "\nArduino connected.\n";
    std::cout << "Press ENTER to start the experiment...\n";

    std::cin.get();

    serialReader.clearInputBuffer();

    std::cout << "Experiment started.\n";
    std::cout << "Press Q to stop after the current step.\n\n";

    SimulationLogger logger(outputFile);
    RealtimePerformanceLogger performanceLogger(performanceOutputFile);

    ExperimentLogger experimentLogger("./validation/realtime_performance/realtime_acquisition.csv", ExperimentLogFormat::Acquisition);

    std::vector<double> currentTemperature = initialTemperature;

    MeasurementSample previousSample = serialReader.readSample();
    experimentLogger.logSample(previousSample);

    double currentTime = 0.0;

    double initialTMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());

    double initialTMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

    logger.logStep(currentTime, previousSample.temperatureC, currentTemperature[centerNodeId], initialTMin, initialTMax);

    std::cout << "Initial furnace temperature: " << previousSample.temperatureC << " C\n";

    for (int step = 1; step <= maxSteps; ++step)
    {

        if (_kbhit())
        {
            const int key = _getch();

            if (key == 'q' || key == 'Q')
            {
                std::cout << "\nExperiment stopped by user.\n";
                break;
            }
        }

        MeasurementSample sample = serialReader.readSample();
        experimentLogger.logSample(sample);

        if (sample.sampleId != previousSample.sampleId + 1)
        {
            throw std::runtime_error(
                "Missing or out-of-order measurement sample");
        }

        const std::uint32_t deltaMs = sample.arduinoTimeMs - previousSample.arduinoTimeMs;

        const double dt = deltaMs / 1000.0;

        if (dt <= 0.0 || dt > 2.0)
        {
            throw std::runtime_error(
                "Invalid measurement time interval");
        }

        const double furnaceTemperature = sample.temperatureC;

        const auto startTime = std::chrono::steady_clock::now();

        auto stepResult = TimeIntegrator::stepNonLinearWithHTC(mesh, ue, material, currentTemperature, dt, boundaryConditions, furnaceTemperature, config.maxPicardIterations, config.picardTolerance);

        const auto endTime = std::chrono::steady_clock::now();

        const double computeTimeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();

        const double computeTimeSeconds = computeTimeMs / 1000.0;

        const double realtimeFactor = computeTimeSeconds > 0.0 ? dt / computeTimeSeconds : 0.0;

        const bool deadlineMissed = computeTimeSeconds > dt;

        currentTemperature = std::move(stepResult.temperature);
        currentTime += dt;

        const double tMin = *std::min_element(currentTemperature.begin(), currentTemperature.end());

        const double tMax = *std::max_element(currentTemperature.begin(), currentTemperature.end());

        logger.logStep(currentTime, furnaceTemperature, currentTemperature[centerNodeId], tMin, tMax);

        performanceLogger.logStep(step, currentTime, computeTimeMs, stepResult.stats.iterations, realtimeFactor, deadlineMissed);

        std::cout
            << "Step " << step
            << " | time = " << currentTime
            << " s | dt = " << dt
            << " s | furnace = " << furnaceTemperature
            << " C | center = " << currentTemperature[centerNodeId]
            << " C | Picard iters = " << stepResult.stats.iterations
            << " | compute = " << computeTimeMs
            << " ms | RTF = " << realtimeFactor
            << (deadlineMissed ? " | Deadline missed!" : "")
            << '\n';

        previousSample = sample;
    }

    std::cout << "\nRealtime session finished.\n";
    std::cout << "Simulation time: " << currentTime << " s\n";
    std::cout << "Results saved.\n";
}
