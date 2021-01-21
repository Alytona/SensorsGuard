# pragma once

# include <string>
# include <mutex>

# include "SensorQueue.hpp"

using namespace std;

class MessagesProducer {

	SensorQueue* pQueue;
	unsigned long Quantity;

	enum ProducerStates {
		NotInited,
		Inited,
		Started,
		Stopping,
		Stopped
	};
	
	ProducerStates State = ProducerStates::NotInited;
	mutex ProducerStateMutex;
	
	ProducerStates getState(); 
	void setState(ProducerStates newState);
	
public :	
	MessagesProducer( SensorQueue* queue_ptr );

	void start();
	void stop();
	
private:
	static void staticWork( MessagesProducer* producer_ptr ) 
	{
		producer_ptr->work();
	}
 
	void work();
};
