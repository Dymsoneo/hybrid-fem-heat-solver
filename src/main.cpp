#include <iostream>


#include "mesh/MeshGenerator.h"
#include "config/SimulationConfig.h"
#include "runner/RealtimeSimulationRunner.h"
#include "io/SerialReader.h"
#include  "io/ExperimentLogger.h"

void testAcquisitionLogging()
{
    SerialReader reader;
    reader.open("COM7", 9600);

    ExperimentLogger logger(
        "acquisition_test.csv",
        ExperimentLogFormat::Acquisition
    );

    constexpr int sampleCount = 20;

    for (int i = 0; i < sampleCount; ++i)
    {
        MeasurementSample sample = reader.readSample();

        logger.logSample(sample);

        std::cout
            << "ID: " << sample.sampleId
            << " | Arduino: " << sample.arduinoTimeMs
            << " ms | PC: " << sample.pcTimeMs
            << " ms | T: " << sample.temperatureC
            << " C\n";
    }

    std::cout << "Acquisition test completed.\n";
}

int main()
{

	testAcquisitionLogging();

	SimulationConfig config = createRealtimeConfig();
	
	Mesh mesh = MeshGenerator::generateCylinderMesh(0.02, 0.05, config.radialElements, config.axialElements);
	UniversalElement ue;

	MaterialModel material(1700.0, "data/materials/magnesium_alloy.csv");

	BoundaryConditionManager boundaryConditions;

	HTCModel furnaceHTC("data/boundary/htc_effective.csv");

	boundaryConditions.setHTCModel(BoundarySide::Right, furnaceHTC);
	boundaryConditions.setHTCModel(BoundarySide::Top, furnaceHTC);


	std::vector<double> initialTemperature(mesh.nodes.size(), 20.0);

	int centerNodeId = mesh.findClosestNode(0.0, 0.025);

	

	RealtimeSimulationRunner::run("COM7", 9600, "validation/realtime_performance/data.csv",
		"validation/realtime_performance/performance.csv",
		mesh, ue, material, boundaryConditions, config, initialTemperature, centerNodeId, 50);

	

	


    return 0;
}