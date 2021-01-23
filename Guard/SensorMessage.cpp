# include <chrono>
# include <ctime>
# include <string>
# include <sstream>
# include <iomanip>

# include <syslog.h>

# include "SensorConfig.hpp"
# include "SensorQueue.hpp"
# include "SensorMessage.hpp"

using namespace std;

void SensorMessage::sendToSyslog(int orderNumber)
{
	syslog(LOG_NOTICE, outMessage(orderNumber).c_str());
}

string SensorMessage::outMessage (int orderNumber)
{
	chrono::time_point<chrono::system_clock> messageTime = pSensorEventData->getTimePoint();
	std::stringstream output;
	output << "[" << outputTime(messageTime) << ":" << orderNumber << "] ";
	output << "'" << pSensorConfig->getName() << "' (pin " << pSensorConfig->getPinNumber() << ") is ";
	output << (pSensorEventData->getSensorState() == 0 ? pSensorConfig->getLowStateName() : pSensorConfig->getHighStateName()) << ".";
	return output.str();
}

string SensorMessage::outputTime (chrono::time_point<chrono::system_clock> timePoint)
{
	chrono::milliseconds milliseconds = 
		chrono::duration_cast< chrono::milliseconds > ( timePoint.time_since_epoch() );
	int ms = milliseconds.count() % 1000;
		
	time_t messageTime = chrono::system_clock::to_time_t( timePoint );
	struct tm* timeinfo = localtime (&messageTime);
		
	std::stringstream output;
	output << setfill('0') << setw(2) << timeinfo->tm_hour << ":";
	output << setfill('0') << setw(2) << timeinfo->tm_min << ":";
	output << setfill('0') << setw(2) << timeinfo->tm_sec << ".";
	output << setfill('0') << setw(3) << ms;
	return output.str();
}
