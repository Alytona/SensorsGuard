# include <chrono>
# include <thread>
# include <syslog.h>

# include "SensorsGuard.hpp"

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
	
	while (getState() != GuardStates::Stopping) {
		this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
	setState( GuardStates::Stopped );

	syslog( LOG_NOTICE, "Sensors Guard is stopping work." );
}
