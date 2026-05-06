#pragma once

// Structure to hold the configuration for the simulation. This includes parameters for the simulation mode, mesh resolution, time stepping, and solver settings.

enum class SimulationMode
{
	Accurate,
	Realtime
};

struct SimulationConfig
{
	SimulationMode mode;

	int radialElements;
	int axialElements;

	double totalTime;
	double timeStep;

	double picardTolerance;
	int maxPicardIterations;

	bool useNonlinearMaterial;
};

SimulationConfig createAccurateConfig();
SimulationConfig createRealtimeConfig();