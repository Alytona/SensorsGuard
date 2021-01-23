# pragma once

# include <chrono>
# include <ctime>
# include <string>

# include "SensorConfig.hpp"
# include "SensorQueue.hpp"
# include "SensorMessage.hpp"

using namespace std;

typedef void (*SensorISRPointer)();

class SensorGuard {

	SensorConfig* pConfig;
	SensorQueue MessagesQueue;
	
public :	
	SensorGuard() 
	{
		pConfig = NULL; 
	}

	void init( SensorConfig* config_ptr, SensorISRPointer isr_ptr );
	void reportState();
	
	bool isActive() {
		return pConfig != NULL && pConfig->isActive();
	}
	SensorMessage* getMessageBefore( chrono::time_point<chrono::system_clock> timePoint );
};


