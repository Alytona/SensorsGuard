
# include <iostream>

# include "SensorConfig.hpp"

SensorConfig::SensorConfig (int pinNumber, string name, bool active) {
	PinNumber = pinNumber;
	Name = name;
	Active = active;
}

SensorConfig::SensorConfig (json sensorJson) {
	PinNumber = sensorJson["PinNumber"];
	Name = sensorJson["Name"];
	Active = sensorJson["Active"];
}

void SensorConfig::print() {
	cout << "Sensor '" << Name << "', pin " << PinNumber << ", " << (isActive() ? "active" : "inactive") << endl;
}

json SensorConfig::serialize() {
	json j;
	j["PinNumber"] = PinNumber;
	j["Name"] = Name;
	j["Active"] = Active;
	return j;
}
	
