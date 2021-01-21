
# pragma once

# include <string>
# include <mutex>

# include "SensorQueue.hpp"

using namespace std;

class MessagesReader {

	SensorQueue* pQueue;
	unsigned long Quantity;
	int LastNumber;
	int ErrorsCounter;

	enum ReaderStates {
		NotInited,
		Inited,
		Started,
		Stopping,
		Stopped
	};
	
	ReaderStates State = ReaderStates::NotInited;
	mutex ReaderStateMutex;
	
	ReaderStates getState(); 
	void setState(ReaderStates newState);
	
public :	
	MessagesReader( SensorQueue* queue_ptr );

	void start();
	void stop();
	
private:
	static void staticWork( MessagesReader* producer_ptr ) 
	{
		producer_ptr->work();
	}
 
	void work();
};
