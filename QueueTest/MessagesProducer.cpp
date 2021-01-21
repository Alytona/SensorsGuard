# include <chrono>
# include <thread>
# include <thread>
# include <iostream>

# include "MessagesProducer.hpp"

using namespace std;

MessagesProducer::ProducerStates MessagesProducer::getState() 
{
	ProducerStates result;
	{
		lock_guard<mutex> lock( ProducerStateMutex );
		result = State;
	}
	return result;
}

void MessagesProducer::setState(ProducerStates newState) 
{
	lock_guard<mutex> lock( ProducerStateMutex );
	State = newState;
}
	
MessagesProducer::MessagesProducer( SensorQueue* queue_ptr ) {
	pQueue = queue_ptr; 
	setState( ProducerStates::Inited );
	Quantity = 0;
}

void MessagesProducer::start() 
{
    std::srand( std::time( nullptr ) );
	
	setState( ProducerStates::Started );
	thread workThread( &staticWork, this );
	workThread.detach();		
}

void MessagesProducer::stop() 
{
	setState( ProducerStates::Stopping );
	while (getState() != ProducerStates::Stopped) {
		this_thread::sleep_for( chrono::milliseconds( 100 ) );
	}
	cout << "Total produced : " << Quantity << endl;
}

void MessagesProducer::work() 
{
	int i = 0;
	while (getState() != ProducerStates::Stopping) {
		
		pQueue->addLast( new SensorQueueElementData( i, (i & 0x01) ) );
		Quantity++;
		
		if (i >= 10000) 
			i = 0;
		cout << "Added message #" << i++ << endl; 

		int delay = rand() % 200 + 10;
		this_thread::sleep_for( chrono::milliseconds( delay ) );
	}
	setState( ProducerStates::Stopped );
}


