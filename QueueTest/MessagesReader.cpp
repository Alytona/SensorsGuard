# include <chrono>
# include <thread>
# include <iostream>

# include "MessagesReader.hpp"

using namespace std;

MessagesReader::ReaderStates MessagesReader::getState() 
{
	ReaderStates result;
	{
		lock_guard<mutex> lock( ReaderStateMutex );
		result = State;
	}
	return result;
}

void MessagesReader::setState(ReaderStates newState) 
{
	lock_guard<mutex> lock( ReaderStateMutex );
	State = newState;
}
	
MessagesReader::MessagesReader( SensorQueue* queue_ptr ) {
	pQueue = queue_ptr; 
	setState( ReaderStates::Inited );
	Quantity = 0;
	LastNumber = -1;
	ErrorsCounter = 0;
}

void MessagesReader::start() 
{
	setState( ReaderStates::Started );
	thread workThread( &staticWork, this );
	workThread.detach();		
}

void MessagesReader::stop() 
{
	setState( ReaderStates::Stopping );
	while (getState() != ReaderStates::Stopped) {
		this_thread::sleep_for( chrono::milliseconds( 100 ) );
	}
	cout << "Total read : " << Quantity << endl;
	if (ErrorsCounter != 0) {
		cout << "Total errors : " << ErrorsCounter << endl;
	}
}

void MessagesReader::work() 
{
	
	while (getState() != ReaderStates::Stopping) {
		
		if (!pQueue->isEmpty()) 
		{
			SensorQueueElementData* queueElementData = pQueue->peekFirst();
			int number = queueElementData->getTimeTicks();
			cout << "Got message #" << number << endl; 
			delete queueElementData;
			Quantity++;
			
			if (LastNumber + 1 != number) {
				cout << "Sequence error!" << endl; 
				ErrorsCounter++;
			}
			LastNumber = number;
		}
		else 
		{
			this_thread::sleep_for( chrono::milliseconds( 500 ) );
		}
	}
	while (!pQueue->isEmpty()) 
	{
		SensorQueueElementData* queueElementData = pQueue->peekFirst();
		cout << "Got message #" << queueElementData->getTimeTicks() << endl; 
		delete queueElementData;
		Quantity++;
	}
	setState( ReaderStates::Stopped );
}


