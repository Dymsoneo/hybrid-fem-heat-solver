#include "SimulationConfig.h"

SimulationConfig createAccurateConfig()
{
    
    return { SimulationMode::Accurate, 20, 40, 500.0, 10.0, 1e-3, 20, true };
    

}

SimulationConfig createRealtimeConfig()
{
	return { SimulationMode::Realtime, 10, 20, 500.0, 10.0, 1e-3, 20, true };
}

