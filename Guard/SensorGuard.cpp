# include <syslog.h>
# include <wiringPi.h>

# include "SensorGuard.hpp"

SensorGuard SensorGuards[29];

void reportState( int guardIndex ) 
{
	SensorGuards[guardIndex].reportState();
}

void reportState_00() { reportState( 0 ); }
void reportState_01() { reportState( 1 ); }
void reportState_02() { reportState( 2 ); }
void reportState_03() { reportState( 3 ); }
void reportState_04() { reportState( 4 ); }
void reportState_05() { reportState( 5 ); }
void reportState_06() { reportState( 6 ); }
void reportState_07() { reportState( 7 ); }
void reportState_08() { reportState( 8 ); }
void reportState_09() { reportState( 9 ); }
void reportState_10() { reportState( 10 ); }
void reportState_11() { reportState( 11 ); }
void reportState_12() { reportState( 12 ); }
void reportState_13() { reportState( 13 ); }
void reportState_14() { reportState( 14 ); }
void reportState_15() { reportState( 15 ); }
void reportState_16() { reportState( 16 ); }
void reportState_17() { reportState( 17 ); }
void reportState_18() { reportState( 18 ); }
void reportState_19() { reportState( 19 ); }
void reportState_20() { reportState( 20 ); }
void reportState_21() { reportState( 21 ); }
void reportState_22() { reportState( 22 ); }
void reportState_23() { reportState( 23 ); }
void reportState_24() { reportState( 24 ); }
void reportState_25() { reportState( 25 ); }
void reportState_26() { reportState( 26 ); }
void reportState_27() { reportState( 27 ); }
void reportState_28() { reportState( 28 ); }

SensorISRPointer SensorISRs[29] = {
	&reportState_00,
	&reportState_01,
	&reportState_02,
	&reportState_03,
	&reportState_04,
	&reportState_05,
	&reportState_06,
	&reportState_07,
	&reportState_08,
	&reportState_09,
	&reportState_10,
	&reportState_11,
	&reportState_12,
	&reportState_13,
	&reportState_14,
	&reportState_15,
	&reportState_16,
	&reportState_17,
	&reportState_18,
	&reportState_19,
	&reportState_20,
	&reportState_21,
	&reportState_22,
	&reportState_23,
	&reportState_24,
	&reportState_25,
	&reportState_26,
	&reportState_27,
	&reportState_28
};

void SensorGuard::init( SensorConfig* config_ptr ) 
{
	pConfig = config_ptr; 
	if (pConfig != NULL && pConfig->isActive()) 
	{
		int pinNumber = pConfig->getPinNumber();
		pinMode( pinNumber, INPUT );
		wiringPiISR( pinNumber, INT_EDGE_BOTH, SensorISRs[ pinNumber - 1 ] );
	}
}
void SensorGuard::reportState() 
{
	int pinNumber = pConfig->getPinNumber();
	syslog( LOG_NOTICE, "Sensor '%s', pin %d, state changed. Current state is %d", pConfig->getName().c_str(), pinNumber, digitalRead( pinNumber ) );
}


