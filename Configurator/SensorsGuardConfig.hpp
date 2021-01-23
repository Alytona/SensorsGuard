# pragma once

# include <iostream>

# include "SensorsConfig.hpp"

using namespace std;

class SensorsGuardConfig {
	
	string ObjectId;
	SensorsConfig Sensors;
	
  public:	

	  string getObjectId() {
		  return ObjectId;
	  }
	  void setObjectId (string objectId ) {
		  ObjectId = objectId;
	  }

	bool configureSensors(int argc, char** argv);
	void getParameter(int argc, char** argv);
	bool setParameter(int argc, char** argv);

	void listSensors();
	
//	int getSensorsQuantity();
	SensorConfig* getSensorConfig(int sensorIndex);

	void serialize (ostream& outputStream);
	void deserialize (istream& inputStream);
};
