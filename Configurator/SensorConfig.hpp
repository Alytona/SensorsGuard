# pragma once

# include <string>
# include "json.hpp"

using json = nlohmann::json;
using namespace std;

enum StatesRegisterKind {
	Low,
	High,
	Both
};
extern const char* StateRegisterKindText[];

class SensorConfig 
{
	bool Active;
	int PinNumber;
	string Name;

	string LowStateName = "low level";
	string HighStateName = "high level";
	StatesRegisterKind StatesToRegister = StatesRegisterKind::Both;

	static string getStatesRegisterKindString(StatesRegisterKind statesRegisterKind) {
		if (statesRegisterKind == StatesRegisterKind::Low)
			return StateRegisterKindText[Low];
		else if (statesRegisterKind == StatesRegisterKind::High)
			return StateRegisterKindText[High];
		return StateRegisterKindText[Both];
	}
	static StatesRegisterKind getStatesRegisterKindValue(string stringValue) {
		if (stringValue.compare(StateRegisterKindText[Low]) == 0)
			return StatesRegisterKind::Low;
		if (stringValue.compare(StateRegisterKindText[High]) == 0)
			return StatesRegisterKind::High;
		return StatesRegisterKind::Both;
	}
	static bool checkStatesRegisterKindString (string stringValue) {
		return (stringValue.compare(StateRegisterKindText[Low]) == 0 ||
			stringValue.compare(StateRegisterKindText[High]) == 0 ||
			stringValue.compare(StateRegisterKindText[Both]) == 0);
	}

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
	inline string getLowStateName () {
		return LowStateName;
	}
	inline void setLowStateName (string stateName) {
		LowStateName = stateName;
	}
	inline string getHighStateName() {
		return HighStateName;
	}
	inline void setHighStateName(string stateName) {
		HighStateName = stateName;
	}

	inline StatesRegisterKind getStatesToRegister() {
		return StatesToRegister;
	}
	void setStatesToRegister(StatesRegisterKind statesToRegister) {
		StatesToRegister = statesToRegister;
	}

	bool setSensorParameter (int argc, char** argv); 

	void print();
	
	json serialize();
};
// ---------------------------------------------------------------------
	
