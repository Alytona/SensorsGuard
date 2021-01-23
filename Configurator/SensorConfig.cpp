
# include <iostream>
# include "SensorConfig.hpp"

const char* StateRegisterKindText[] = {
	[Low] = "low", [High] = "high", [Both] = "both"
};

SensorConfig::SensorConfig (int pinNumber, string name, bool active) {
	PinNumber = pinNumber;
	Name = name;
	Active = active;
}

SensorConfig::SensorConfig (json sensorJson) {
	
	PinNumber = sensorJson["PinNumber"];
	Name = sensorJson["Name"];
	Active = sensorJson["Active"];
	
	if (!sensorJson["StatesToRegister"].empty())
		StatesToRegister = getStatesRegisterKindValue( sensorJson["StatesToRegister"] );
	if (!sensorJson["LowStateName"].empty())
		LowStateName = sensorJson["LowStateName"];
	if (!sensorJson["HighStateName"].empty())
		HighStateName = sensorJson["HighStateName"];
}

bool SensorConfig::setSensorParameter(int argc, char** argv) 
{
	string parameterName(argv[0]);
	string parameterValue(argv[1]);

	if (parameterName.compare("states_to_reg") == 0) {
		if (checkStatesRegisterKindString( parameterValue )) {
			setStatesToRegister( getStatesRegisterKindValue( parameterValue ) );
		}
		else {
			cout << "Unknown value '" << parameterValue << "' of sensor parameter states_to_reg." << endl;
			return false;
		}
	}
	else
	if (parameterName.compare("low_level_name") == 0) {
		setLowStateName(argv[1]);
	}
	else
	if (parameterName.compare("high_level_name") == 0) {
		setHighStateName(argv[1]);
	} else {
		cout << "Unknown sensor parameter '" << parameterName << "'." << endl;
		return false;
	}
	return true;
}

void SensorConfig::print() {
	cout << "Sensor '" << Name << "', pin " << PinNumber << ", " << (isActive() ? "active" : "inactive") << endl;
	cout << "\t States to register : " << StateRegisterKindText[StatesToRegister] << endl;
	cout << "\t Low state name : " << LowStateName << endl;
	cout << "\t High state name : " << HighStateName << endl;
}

json SensorConfig::serialize() {
	json j;
	j["PinNumber"] = PinNumber;
	j["Name"] = Name;
	j["Active"] = Active;
	j["StatesToRegister"] = getStatesRegisterKindString( StatesToRegister );
	j["LowStateName"] = LowStateName;
	j["HighStateName"] = HighStateName;

	return j;
}
	
