# pragma once

# include <iostream>

# include "SensorsConfig.hpp"

using namespace std;

class SensorsGuardConfig {
	
	SensorsConfig Sensors;
	
  public:	
	bool configureSensors(int argc, char** argv);

	void listSensors();
	
	int getSensorsQuantity();
	SensorConfig* getSensorConfig(int sensorIndex);

	void serialize (ostream& outputStream);
	void deserialize (istream& inputStream);
};
