#include <iostream>

#include "boundary/HTCModel.h"
#include "boundary/BoundaryConditionManager.h"
#include "solver/SimulationRunner.h"
#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "io/ResultExporter.h"
#include "io/ExperimentLogger.h"
#include "io/SimulationLogger.h"
#include "io/MeasurementReader.h"

int main()
{
   
	MeasurementReader reader("data/experiments/experiment_001/measurements.csv");

	std::cout << "Loaded " << reader.size() << " measurement samples." << std::endl;

	while (reader.hasNext())
	{
		MeasurementSample sample = reader.next();

		std::cout << "t = " << sample.timeSeconds << " s, furnace = " << sample.furnaceTemperature << " °C" << std::endl;
	}

    return 0;
}