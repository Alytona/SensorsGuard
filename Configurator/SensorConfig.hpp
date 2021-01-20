# pragma once

# include <string>

# include "json.hpp"

using json = nlohmann::json;

using namespace std;

class SensorConfig {
	int PinNumber;
	string Name;
	bool Active;
	
  public:
	SensorConfig (int pinNumber, string name, bool active);
	SensorConfig (json sensorJson);
	
	inline int getPinNumber() {
		return PinNumber;
	}
	inline void setPinNumber(int newPinNumber) {
		PinNumber = newPinNumber;
	}

	inline string getName () {
		return Name;
	}
	inline void setName (string name) {
		Name = name;
	}

	inline bool isActive() {
		return Active;
	}
	inline void activate() {
		Active = true;
	}
	inline void deactivate() {
		Active = false;
	}
	
	void print();
	
	json serialize();
};
// ---------------------------------------------------------------------
	
