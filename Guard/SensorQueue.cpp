# include <mutex>
# include <string>
# include <ctime>
# include <chrono>

# include "SensorQueue.hpp"

using namespace std;

SensorQueueElement* SensorQueue::peekFirst()
{
	if (isEmpty())
		return NULL;
		
	SensorQueueElement* firstElement = FirstElement;

	FirstElement = FirstElement->getNextElement();

	return firstElement;
}
bool SensorQueue::isEmpty()
{
	SensorQueueElement* lastElement;
	{
		lock_guard<mutex> lock( LastElementMutex );
		lastElement = LastElement;
	}
	return FirstElement == lastElement;
}
void SensorQueue::addLast( SensorQueueElementData* data )
{
	LastElement->setData( data );
	LastElement->setNextElement( new SensorQueueElement() );
	{
		lock_guard<mutex> lock( LastElementMutex );
		LastElement = LastElement->getNextElement();
	}
}


