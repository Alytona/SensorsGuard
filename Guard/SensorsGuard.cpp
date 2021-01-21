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
	
	for (int i = 0; i < 32; i++) 
	{
		cout << "wPi pin number : " << i << endl;

		if (i >= 0 && i < 64) 
		{
			int bcmPin = wpiPinToGpio( i );
			cout << "BCM pin number : " << bcmPin << endl;

			if (bcmPin > 0 && bcmPin <= 31 || bcmPin >= 192) 
			{
			    SensorConfig* sensorConfig = pConfig->getSensorConfig( i );
				if (sensorConfig != NULL) 
				{
					SensorGuards[i].init( sensorConfig );
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
			} else {
			    cout << "Incorrect BCM pin number" << endl;
			}
		} else {
		    cout << "Incorrect pin number" << endl;
		}
	}
	
	while (getState() != GuardStates::Stopping) {
		this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
	setState( GuardStates::Stopped );

	syslog( LOG_NOTICE, "Sensors Guard is stopping work." );
}
