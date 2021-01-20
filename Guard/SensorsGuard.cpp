# include <chrono>
# include <thread>
# include <syslog.h>

# include <wiringPi.h>

# include "SensorsGuard.hpp"
# include "SensorGuard.hpp"

SensorsGuard::GuardStates SensorsGuard::getState() 
{
	GuardStates result;
	{
		lock_guard<mutex> lock( GuardStateMutex );
		result = State;
	}
	return result;
}

void SensorsGuard::setState(GuardStates newState) 
{
	lock_guard<mutex> lock( GuardStateMutex );
	State = newState;
}
	
SensorsGuard::SensorsGuard( SensorsGuardConfig* config_ptr ) {
	pConfig = config_ptr; 
	setState( GuardStates::Inited );
}

void SensorsGuard::start() 
{
	syslog( LOG_NOTICE, "Sensors Guard will start a work thread." );
	
	setState( GuardStates::Started );
	thread workThread( &staticWork, this );
	workThread.detach();		

	syslog( LOG_NOTICE, "Sensors Guard started the work thread." );
}

void SensorsGuard::stop() 
{
	syslog( LOG_NOTICE, "Sensors Guard will stop work." );

	setState( GuardStates::Stopping );
	while (getState() != GuardStates::Stopped) {
		this_thread::sleep_for( chrono::milliseconds( 100 ) );
	}
	syslog( LOG_NOTICE, "Sensors Guard stopped work." );
}

void SensorsGuard::work() 
{
	syslog( LOG_NOTICE, "Sensors Guard started work." );
	
	wiringPiSetup();
	
//	SensorGuard sensors[29];
	
	for (int i = 1; i <= 29; i++) 
	{
		SensorConfig* sensorConfig = pConfig->getSensorConfig( i );
		if (sensorConfig != NULL) 
		{
			SensorGuards[i - 1].init( sensorConfig );
			if (sensorConfig->isActive())
			{
				pinMode( i, INPUT );
				syslog( LOG_NOTICE, "Active sensor '%s', pin %d, state %d", sensorConfig->getName().c_str(), i, digitalRead( i ) );
			}
			else 
			{
				syslog( LOG_NOTICE, "Inactive sensor '%s', pin %d", sensorConfig->getName().c_str(), sensorConfig->getPinNumber() );
			}
		}
	}
	
	while (getState() != GuardStates::Stopping) {
		this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
	setState( GuardStates::Stopped );

	syslog( LOG_NOTICE, "Sensors Guard is stopping work." );
}
