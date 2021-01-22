# pragma once

# include <chrono>
# include <ctime>
# include <string>
# include "SensorConfig.hpp"
# include "SensorQueue.hpp"

# include <sys/timeb.h>

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
	void sendToSyslog( int orderNumber ) 
	{
		chrono::time_point<chrono::system_clock> messageTime = pSensorEventData->getTimePoint();
		
		cout << "Syslog: pin " << pSensorConfig->getPinNumber() << " at \t";
		outputTime( messageTime );
		cout << endl;
/*		
		syslog( LOG_NOTICE, "[%s:%d] Sensor '%s', pin %d, state changed. Current state is %d", 
			ctime( &messageTime ), orderNumber, 
			pSensorConfig->getName().c_str(), pSensorConfig->getPinNumber(), 
			pSensorEventData->getSensorState() );
*/
	}
	
	static void outputTime (chrono::time_point<chrono::system_clock> timePoint) 
	{
		chrono::milliseconds milliseconds = 
			chrono::duration_cast< chrono::milliseconds > ( timePoint.time_since_epoch() );
		int ms = milliseconds.count() % 1000;
		
		time_t messageTime = chrono::system_clock::to_time_t( timePoint );
		struct tm* timeinfo = localtime (&messageTime);
		
		cout << setfill('0') << setw(2) << timeinfo->tm_hour << ":";
		cout << setfill('0') << setw(2) << timeinfo->tm_min << ":";
		cout << setfill('0') << setw(2) << timeinfo->tm_sec << ".";
		cout << setfill('0') << setw(3) << ms;
	}
};

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


