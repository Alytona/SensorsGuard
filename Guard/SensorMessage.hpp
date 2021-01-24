# pragma once

# include <chrono>
# include <ctime>
# include <string>

# include "SensorConfig.hpp"
# include "SensorQueue.hpp"

using namespace std;

typedef void (*SensorISRPointer)();

class SensorMessage 
{
	SensorConfig* pSensorConfig;
	SensorQueueElementData* pSensorEventData;
	
public:
	SensorMessage (SensorConfig* sensorConfigPtr, SensorQueueElementData* sensorEventDataPtr) 
	{
		pSensorConfig = sensorConfigPtr;
		pSensorEventData = sensorEventDataPtr;
	}
	~SensorMessage() 
	{
		if (pSensorEventData != NULL)
			delete pSensorEventData;
	}
	chrono::time_point<chrono::system_clock> getTimePoint() 
	{
		return pSensorEventData->getTimePoint();
	}
	void sendToSyslog( int orderNumber );

	static string outputTime( chrono::time_point<chrono::system_clock> timePoint );

private:
	string outMessage( int orderNumber );
};


