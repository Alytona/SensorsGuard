
# include "json.hpp"
# include "SensorsGuardConfig.hpp"

using json = nlohmann::json;
using namespace std;

bool SensorsGuardConfig::configureSensors(int argc, char** argv) {
	cout << "Sensors configuration." << endl;
//		for (int i=0; i<argc; i++)
//		{
//			cout << argv[i] << endl;
//		}
	if (argc >= 2) {
		string command(argv[0]);
		
		if (command.compare("add") == 0) {
			return Sensors.add(argc - 1, argv + 1);
		} 
		else 
		if (command.compare("rename") == 0) {
			return Sensors.rename(argc - 1, argv + 1);
		}
		else 
		if (command.compare("activate") == 0) {
			return Sensors.activate(argc - 1, argv + 1);
		}
		else 
		if (command.compare("deactivate") == 0) {
			return Sensors.deactivate(argc - 1, argv + 1);
		}
		else 
		if (command.compare("move") == 0) {
			return Sensors.move(argc - 1, argv + 1);
		}
		else 
		if (command.compare("remove") == 0) {
			return Sensors.remove(argc - 1, argv + 1);
		} 
		else {
			cout << "Unknown sensors configuration command '" << command << "'." << endl;
		}
	}
	else {
		listSensors();
	}
	return false;
}

void SensorsGuardConfig::listSensors() {
		Sensors.list();
}

void SensorsGuardConfig::serialize (ostream& outputStream) {
	json j;
	j["Sensors"] = Sensors.serialize();
	outputStream << j.dump(2) << endl;
}
void SensorsGuardConfig::deserialize (istream& inputStream) {
	json j;
	inputStream >> j;
	Sensors.deserialize(j["Sensors"]);
}

int SensorsGuardConfig::getSensorsQuantity() 
{
}

SensorConfig* SensorsGuardConfig::getSensorConfig(int sensorIndex) 
{
	return Sensors.getSensorConfig( sensorIndex );
}

