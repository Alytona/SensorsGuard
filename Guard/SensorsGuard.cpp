# include <map>
# include <chrono>
# include <thread>
# include <syslog.h>

# include <wiringPi.h>

# include "SensorsGuard.hpp"
# include "SensorGuard.hpp"

# define SENSORS_QUANTITY 32

SensorGuard SensorGuards[SENSORS_QUANTITY];

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
void reportState_29() { reportState( 29 ); }
void reportState_30() { reportState( 30 ); }
void reportState_31() { reportState( 31 ); }

SensorISRPointer SensorISRs[SENSORS_QUANTITY] = {
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
	&reportState_28,
	&reportState_29,
	&reportState_30,
	&reportState_31
};

void SensorsGuard::randomEventsGenerator() 
{
    std::srand( std::time( 0 ) );
	
	while (getState() != GuardStates::Stopping) 
	{
		int sensorIndex = rand() % 31;
		int messagesQuantity = rand() % 5;
		
		for (int i=0; i<messagesQuantity && getState() != GuardStates::Stopping; i++) {
			this_thread::sleep_for( chrono::milliseconds( rand() % 100 + 10 ) );
			if (SensorGuards[sensorIndex].isActive())
				SensorGuards[sensorIndex].reportState();
		}
		
//		this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
}

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

	// thread randomGeneratorThread( &staticRandomEventsGenerator, this );
	// randomGeneratorThread.detach();		

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
	
	for (int i = 0; i < SENSORS_QUANTITY; i++) 
	{
		SensorConfig* sensorConfig = pConfig->getSensorConfig( i );
		if (sensorConfig != NULL) 
		{
			if (checkPinNumber( i )) 
			{
				SensorGuards[i].init( sensorConfig, SensorISRs[i] );
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
	}
	
	while (getState() != GuardStates::Stopping) {
		if (transferMessages() == 0)
			this_thread::sleep_for( chrono::milliseconds( 1000 ) );
		else 
			this_thread::sleep_for( chrono::milliseconds( 100 ) );
	}
	setState( GuardStates::Stopped );

	syslog( LOG_NOTICE, "Sensors Guard is stopping work." );
}

bool SensorsGuard::checkPinNumber (int pinNumber) 
{
	if (pinNumber < 0 && pinNumber > 63) 
	{
		cout << "Incorrect pin number '" << pinNumber << "'" << endl;
		return false;
	}
	
	int bcmPin = wpiPinToGpio( pinNumber );
	if (bcmPin > 0 && bcmPin <= 31 || bcmPin >= 192)
		return true;
		
	cout << "Incorrect BCM pin number '" << bcmPin << "'" << endl;
	return false; 
}

int SensorsGuard::transferMessages() 
{
	chrono::time_point<chrono::system_clock> transferStartMoment = chrono::system_clock::now();
	
	typedef multimap < chrono::time_point<chrono::system_clock>, SensorMessage* > MessagesMap;
	MessagesMap messages;
	
	for (int i = 0; i < SENSORS_QUANTITY; i++) 
	{
		if (SensorGuards[i].isActive()) 
		{
			SensorMessage* message = SensorGuards[i].getMessageBefore( transferStartMoment );
			while (message != NULL) 
			{
				messages.insert( MessagesMap::value_type( message->getTimePoint(), message ) );
				message = SensorGuards[i].getMessageBefore( transferStartMoment );
			}
		}
		
		// Testing delay
		// this_thread::sleep_for( chrono::milliseconds( 10 ) );
	}

	int messagesCounter = 0;
	for (MessagesMap::iterator iM = messages.begin(); iM != messages.end(); iM++) 
	{
		SensorMessage* message = iM->second;
		message->sendToSyslog( messagesCounter++ );
		delete message;
	}
/*
	cout << "Transfer at \t\t";
	SensorMessage::outputTime( transferStartMoment );
	cout << endl;
	
	transferStartMoment = chrono::system_clock::now();
	cout << "Transfer end at \t\t";
	SensorMessage::outputTime( transferStartMoment );
	cout << endl;
*/	
	return messagesCounter;
}

