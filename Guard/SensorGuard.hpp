# pragma once

# include <string>
# include "SensorConfig.hpp"

using namespace std;

typedef void (*SensorISRPointer)();

class SensorGuard {

	SensorConfig* pConfig;
	
public :	
	SensorGuard() 
	{
		pConfig = NULL; 
	}
	void init( SensorConfig* config_ptr, SensorISRPointer isr_ptr );
	void reportState();
};


