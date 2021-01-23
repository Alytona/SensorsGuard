# pragma once

# include <mutex>
# include <string>
# include <ctime>
# include <chrono>

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

	bool isEmpty();
	SensorQueueElement* peekFirst();
	void addLast(SensorQueueElementData* data);
};


