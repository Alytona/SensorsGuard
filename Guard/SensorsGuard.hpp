# pragma once

# include <string>
# include <mutex>

# include "SensorsGuardConfig.hpp"

using namespace std;

class SensorsGuard {

	SensorsGuardConfig* pConfig;

	enum GuardStates {
		NotInited,
		Inited,
		Started,
		Stopping,
		Stopped
	};
	
	GuardStates State = GuardStates::NotInited;
	mutex GuardStateMutex;
	
	GuardStates getState(); 
	void setState(GuardStates newState);
	
public :	
	SensorsGuard( SensorsGuardConfig* config_ptr );

	void start();
	void stop();
	
private:
	static void staticWork( SensorsGuard* guard_ptr ) 
	{
		guard_ptr->work();
	}
	static void staticRandomEventsGenerator( SensorsGuard* guard_ptr ) 
	{
		guard_ptr->randomEventsGenerator();
	}
 
	void randomEventsGenerator();
	void work();
	
	bool checkPinNumber (int pinNumber);
	
	int transferMessages();
};
