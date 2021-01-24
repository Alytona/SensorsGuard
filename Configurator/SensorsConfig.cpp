
# include "SensorsConfig.hpp"

using std::cout;
using std::endl;

const int MAX_PIN_NUMBER = 31;

typedef map<int, SensorConfig*>::iterator SensorsIterator;

SensorsConfig::~SensorsConfig() {
	for(map<int, SensorConfig*>::iterator iSensor = Sensors.begin(); iSensor != Sensors.end(); iSensor++ ) {
		delete iSensor->second;
	}
}

int SensorsConfig::getPinNumber (string pinNumberStr) 
{
	try {
		int pinNumber = stoul(pinNumberStr);
		if (pinNumber >= 0 && pinNumber <= MAX_PIN_NUMBER) {
			return pinNumber;
		} else {
			cout << "The pin number '" + pinNumberStr + "' is incorrect. ";
			cout << "It should be an integer value in range from 0 to " << MAX_PIN_NUMBER << "." << endl;
		}
	} catch (exception&)  {
		cout << "The pin number '" + pinNumberStr + "' is incorrect. ";
		cout << "It should be an integer value in range from 0 to " << MAX_PIN_NUMBER << "." << endl;
	}
	return -1;
}

bool SensorsConfig::insertSensor(SensorConfig* pSensor) {
	pair< map<int, SensorConfig*>::iterator, bool > result = 
		Sensors.insert(	map<int, SensorConfig*>::value_type( pSensor->getPinNumber(), pSensor));
	return result.second;
}

bool SensorsConfig::add(int argc, char** argv) 
{
	cout << "Adding sensor." << endl;
	if (argc != 3) {
		cout << "Add command should have 3 parameters (<pin number> <name> <state>)." << endl;
	}
	else {
		int pinNumber = getPinNumber(argv[0]);
		if (pinNumber != -1) {
			SensorsIterator iSensor = Sensors.find(pinNumber);
			if (iSensor != Sensors.end()) {
				cout << "Sensor with pin number " << argv[0] << " already exists." << endl;
			}
			else {
				string activeStr(argv[1]);
				bool active = (activeStr.compare("active") == 0);
				
				if (insertSensor(new SensorConfig( pinNumber, argv[2], active ))) {
					cout << "Sensor was added." << endl;
					return true;
				} else {
					cout << "Sensor was not added. Check add command parameters (<pin number> <name> <state>)." << endl;
				}
			}
		}
	}
	return false;
}

bool SensorsConfig::set(int argc, char** argv)
{
	cout << "Setting parameter of sensor." << endl;
	if (argc < 3) {
		cout << "Set parameter command should have at least 3 parameters (<pin number> <parameter name> <value>)." << endl;
	}
	else {
		SensorConfig* sensorConfig = getSensorConfig(argv[0]);
		if (sensorConfig != NULL && sensorConfig->setSensorParameter( argc-1, argv+1 )) {
			cout << "The parameter of sensor successfully set." << endl;
			return true;
		}
	}
	return false;
}

SensorConfig* SensorsConfig::getSensorConfig (string pinNumberStr) 
{
	int pinNumber = getPinNumber(pinNumberStr);
	SensorConfig* sensorConfig = getSensorConfig( pinNumber );
	if (sensorConfig == NULL)
		cout << "Sensor with pin number " << pinNumber << " does not exists." << endl;
	return sensorConfig;
}

SensorConfig* SensorsConfig::getSensorConfig (int pinNumber) 
{
	if (pinNumber != -1) {
		SensorsIterator iSensor = Sensors.find(pinNumber);
		if (iSensor == Sensors.end()) {
			return NULL;
		}
		return iSensor->second;
	}
	return NULL;
}

bool SensorsConfig::rename(int argc, char** argv) 
{
	cout << "Renaming sensor." << endl;
	if (argc != 2) {
		cout << "Rename command should have 2 parameters (<pin number> <name>)." << endl;
	}
	else {
		SensorConfig *sensorConfig = getSensorConfig( argv[0] );
		if (sensorConfig != NULL) {
			sensorConfig->setName( argv[1] );
			cout << "Sensor was renamed." << endl;
			return true;
		}
	}
	return false;
}

bool SensorsConfig::activate(int argc, char** argv) 
{
	cout << "Activating sensor." << endl;
	if (argc != 1) {
		cout << "Activate command should have 1 parameter (<pin number>)." << endl;
	}
	else {
		SensorConfig *sensorConfig = getSensorConfig( argv[0] );
		if (sensorConfig != NULL) {
			if (sensorConfig->isActive()){
				cout << "Sensor " << argv[0] << " is already active." << endl;
			} else {
				sensorConfig->activate();
				cout << "Sensor was activated." << endl;
				return true;
			}
		}
	}
	return false;
}

bool SensorsConfig::deactivate(int argc, char** argv) 
{
	cout << "Deactivating sensor." << endl;
	if (argc != 1) {
		cout << "Deactivate command should have 1 parameter (<pin number>)." << endl;
	}
	else {
		SensorConfig *sensorConfig = getSensorConfig( argv[0] );
		if (sensorConfig != NULL) {
			if (!sensorConfig->isActive()){
				cout << "Sensor " << argv[0] << " is already inactive." << endl;
			} else {
				sensorConfig->deactivate();
				cout << "Sensor was deactivated." << endl;
				return true;
			}
		}
	}
	return false;
}

bool SensorsConfig::move(int argc, char** argv) {
	cout << "Changing sensors pin number." << endl;
	if (argc != 2) {
		cout << "Move command should have 2 parameters (<old pin number> <new pin number>)." << endl;
	}
	else {
		int oldPinNumber = getPinNumber( argv[0] );
		int newPinNumber = getPinNumber( argv[1] );
		if (oldPinNumber != -1 && newPinNumber != -1) {
			SensorsIterator iNewSensor = Sensors.find( newPinNumber );
			if (iNewSensor != Sensors.end()) {
				cout << "Sensor with pin number " << argv[1] << " already exists." << endl;
			} else {
				SensorsIterator iOldSensor = Sensors.find( oldPinNumber );
				if (iOldSensor == Sensors.end()) {
					cout << "Sensor with pin number " << argv[0] << " does not exists." << endl;
				}
				else {
					SensorConfig* pSensorConfig = iOldSensor->second;
					Sensors.erase( iOldSensor );
					
					pSensorConfig->setPinNumber( newPinNumber );
					if (insertSensor(pSensorConfig)) {
						cout << "Sensor was moved." << endl;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool SensorsConfig::remove(int argc, char** argv) {
	cout << "Deleting sensor." << endl;
	if (argc != 1) {
		cout << "Remove command should have 1 parameter (<pin number>)." << endl;
	}
	else {
		int pinNumber = getPinNumber( argv[0] );
		if (pinNumber != -1) {
			SensorsIterator iSensor = Sensors.find( pinNumber );
			if (iSensor == Sensors.end()) {
				cout << "Sensor with pin number " << argv[0] << " does not exists." << endl;
			}
			else {
				delete iSensor->second;
				Sensors.erase( iSensor );
				cout << "Sensor was removed." << endl;
				return true;
			}
		}
	}
	return false;
}

void SensorsConfig::list() {
	cout << "There are " << Sensors.size() << " sensors." << endl;
	for(map<int, SensorConfig*>::iterator iSensor = Sensors.begin(); iSensor != Sensors.end(); iSensor++ ) {
		iSensor->second->print();
	}
}

json SensorsConfig::serialize() {
	json j;
	for(map<int, SensorConfig*>::iterator iSensor = Sensors.begin(); iSensor != Sensors.end(); iSensor++ ) {
		j.push_back(iSensor->second->serialize());
	}
	return j;
}

void SensorsConfig::deserialize (json sensorsJson) {
	for (auto& sensorJson : sensorsJson) {
		insertSensor( new SensorConfig( sensorJson ) );
	}
}
