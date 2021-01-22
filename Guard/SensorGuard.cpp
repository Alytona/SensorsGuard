# include <chrono>

# include <syslog.h>
# include <wiringPi.h>

# include "SensorGuard.hpp"

void SensorGuard::init( SensorConfig* config_ptr, SensorISRPointer isr_ptr ) 
{
	pConfig = config_ptr; 
	if (pConfig != NULL && pConfig->isActive()) 
	{
		int pinNumber = pConfig->getPinNumber();
		pinMode( pinNumber, INPUT );
		wiringPiISR( pinNumber, INT_EDGE_BOTH, isr_ptr );
	}
}
void SensorGuard::reportState() 
{
	chrono::time_point<chrono::system_clock> messageTimePoint = chrono::system_clock::now();
		
	int pinNumber = pConfig->getPinNumber();
	int state = digitalRead( pinNumber );

	cout << "Event: pin " << pinNumber << " at \t";
	SensorMessage::outputTime( messageTimePoint );
	cout << endl;
	
//	syslog( LOG_NOTICE, "Sensor '%s', pin %d, state changed. Current state is %d", 
//		pConfig->getName().c_str(), pinNumber, state );
		
	MessagesQueue.addLast( new SensorQueueElementData( messageTimePoint, state ) );
}

SensorMessage* SensorGuard::getMessageBefore( chrono::time_point<chrono::system_clock> timePoint ) 
{
	SensorQueueElementData* firstMessageData = MessagesQueue.getFirst();
	if (firstMessageData != NULL && firstMessageData->getTimePoint() <= timePoint) 
	{
		MessagesQueue.peekFirst();
		return new SensorMessage( pConfig, firstMessageData );
	}
	return NULL;
}

