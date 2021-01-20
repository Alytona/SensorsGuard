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
	void init( SensorConfig* config_ptr );
	void reportState();
};

extern SensorGuard SensorGuards[29];


