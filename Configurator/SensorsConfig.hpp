# pragma once

# include <string>
# include <map>

# include "json.hpp"

# include "SensorConfig.hpp"

using json = nlohmann::json;
using namespace std;

class SensorsConfig {
	map<int, SensorConfig*> Sensors;
	
  public:	
	~SensorsConfig();

	bool add(int argc, char** argv);
	bool set(int argc, char** argv);
	bool rename(int argc, char** argv);
	bool activate(int argc, char** argv);
	bool deactivate(int argc, char** argv);
	bool move(int argc, char** argv);
	bool remove(int argc, char** argv);
	void list();

	json serialize();
	void deserialize (json sensorsJson);

	SensorConfig* getSensorConfig (int pinNumber);

  private:
	bool insertSensor(SensorConfig* pSensor);

	int getPinNumber (string pinNumberStr);
	SensorConfig* getSensorConfig (string pinNumberStr);
};
