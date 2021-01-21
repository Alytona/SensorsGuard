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
	int pinNumber = pConfig->getPinNumber();
	syslog( LOG_NOTICE, "Sensor '%s', pin %d, state changed. Current state is %d", pConfig->getName().c_str(), pinNumber, digitalRead( pinNumber ) );
}


