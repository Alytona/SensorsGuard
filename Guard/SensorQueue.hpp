# pragma once

# include <mutex>
# include <string>

// # include "SensorConfig.hpp"

using namespace std;

class SensorQueueElementData {
	
	int TimeTicks;
	int SensorState;
	
public:
	SensorQueueElementData (int timeTicks, int sensorState) 
	{
		TimeTicks = timeTicks;
		SensorState = sensorState;
	}
	int getTimeTicks() {
		return TimeTicks;
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
	SensorQueueElementData* peekFirst() 
	{
		if (isEmpty())
			return NULL;
		
		SensorQueueElement* firstElement = FirstElement;
		SensorQueueElementData* firstElementData = FirstElement->getData();

		FirstElement = FirstElement->getNextElement();

		delete firstElement;
		return firstElementData;
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


