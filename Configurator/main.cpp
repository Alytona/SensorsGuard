
# include <iostream>
# include <fstream>
# include <string>
# include <typeinfo>

# include "SensorsGuardConfig.hpp"

using namespace std;

void printUsage() {
	cout << "Sensors Guard configurator usage:" << endl;
	cout << "SensorsGuardConfig sensor <command>" << endl;
	cout << "Commands:" << endl;
	cout << "\tadd <pin number> active|inactive \"<sensor name>\" - add sensor" << endl;
	cout << "\trename <pin number> \"<new sensor name>\" - rename sensor" << endl;
	cout << "\tactivate <pin number> - activate sensor" << endl;
	cout << "\tdeactivate <pin number> - deactivate sensor" << endl;
	cout << "\tmove <pin number> <new pin number> - change sensors pin number" << endl;
	cout << "\tremove <pin number> - delete sensor" << endl;
}

int main(int argc, char** argv) 
{
	const char* configFilename = "sensors_guard.cfg";
	
	try
	{
		for (int i=0; i<argc; i++)
		{
			cout << i << ":\t" << argv[i] << endl;
		}

		if (argc <= 1) {
			printUsage();
		}
		else 
		{
			std::ifstream inConfigFile( configFilename );
			if (inConfigFile.is_open())
			{
				SensorsGuardConfig config;
				config.deserialize( inConfigFile );
				inConfigFile.close();

				bool changed = false;
				string argv1( argv[1] );
				if (argv1.compare("sensor") == 0) {
					changed = config.configureSensors(argc - 2, argv + 2);
				} else {
					printUsage();
				}

				if (changed) {
					cout << "SensorGuard config changed." << endl;
					config.serialize( cout );
					
					std::ofstream outConfigFile( configFilename );
					if (outConfigFile.is_open()) {
						config.serialize( outConfigFile );
						outConfigFile.close();
					}
					else 
					{
						cout << "Can not open config file " << configFilename << " to save." << endl;
					}
				}
			}
			else {
				cout << "Can not open config file " << configFilename << " to read." << endl;
			}
		}
	}
	catch (exception& error) {
		cout << typeid( error ).name() << " - " << error.what() << endl;
	}
}
// ---------------------------------------------------------------------
	
