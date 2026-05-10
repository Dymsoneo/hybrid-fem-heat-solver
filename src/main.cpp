#include <iostream>

#include "boundary/HTCModel.h"
#include "boundary/BoundaryConditionManager.h"
#include "solver/SimulationRunner.h"
#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "io/ResultExporter.h"
#include "io/ExperimentLogger.h"
#include "io/SimulationLogger.h"

int main()
{
    ExperimentLogger experimentLogger(
        "data/experiments/experiment_001/measurements.csv"
    );

    SimulationLogger simulationLogger(
        "data/experiments/experiment_001/realtime_simulation.csv"
    );

    experimentLogger.logMeasurement(0.0, 20.0);
    experimentLogger.logMeasurement(1.0, 22.5);
    experimentLogger.logMeasurement(2.0, 25.1);

    simulationLogger.logStep(0.0, 20.0, 20.0, 20.0, 20.0);
    simulationLogger.logStep(1.0, 22.5, 20.12, 20.0, 20.30);
    simulationLogger.logStep(2.0, 25.1, 20.31, 20.0, 20.80);


    return 0;
}