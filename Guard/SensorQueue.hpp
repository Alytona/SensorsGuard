# pragma once

# include <mutex>
# include <string>
# include <ctime>
# include <chrono>

// # include "SensorConfig.hpp"

using namespace std;

class SensorQueueElementData {
	
	chrono::time_point<chrono::system_clock> TimePoint;
	int SensorState;
	
public:
	SensorQueueElementData (chrono::time_point<chrono::system_clock> timePoint, int sensorState) 
	{
		TimePoint = timePoint;
		SensorState = sensorState;
	}
	chrono::time_point<chrono::system_clock> getTimePoint() {
		return TimePoint;
	}
	int getSensorState() {
		return SensorState;
	}
};

class SensorQueueElement {
	
	SensorQueueElementData* Data;
	SensorQueueElement* NextElement;
	
public:
	SensorQueueElement () 
	{
		Data = NULL;
		NextElement = NULL;
	}
	SensorQueueElementData* getData() {
		return Data;
	}
	void setData (SensorQueueElementData* data) {
		Data = data;
	}
	SensorQueueElement* getNextElement () {
		return NextElement;
	}
	void setNextElement (SensorQueueElement* nextElement) {
		NextElement = nextElement;
	}
};

class SensorQueue {

	SensorQueueElement* FirstElement;
	SensorQueueElement* LastElement;
	
	mutex LastElementMutex;
	
public :	
	SensorQueue() 
	{
		FirstElement = LastElement = new SensorQueueElement();
	}

	SensorQueueElementData* getFirst() 
	{
		return FirstElement->getData();
	}
	SensorQueueElement* peekFirst() 
	{
		if (isEmpty())
			return NULL;
		
		SensorQueueElement* firstElement = FirstElement;

		FirstElement = FirstElement->getNextElement();

		return firstElement;
	}
	bool isEmpty() 
	{
		SensorQueueElement* lastElement;
		{
			lock_guard<mutex> lock( LastElementMutex );
			lastElement = LastElement;
		}
		return FirstElement == lastElement;
	}
	void addLast( SensorQueueElementData* data ) 
	{
		LastElement->setData( data );
		LastElement->setNextElement( new SensorQueueElement() );
		{
			lock_guard<mutex> lock( LastElementMutex );
			LastElement = LastElement->getNextElement();
		}
	}
};


