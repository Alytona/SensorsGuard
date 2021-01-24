# include <sys/types.h>
# include <unistd.h>

# include <iostream>
# include <fstream>
# include <string>
# include <typeinfo>
# include <syslog.h>
# include <chrono>
# include <thread>
# include <signal.h>

// # include <process.h>

# include "SensorsGuardConfig.hpp"
# include "SensorsGuard.hpp"
# include "StopFile.hpp"

using namespace std;

StopFile stopFile( "./stopSensorsGuard" );

void printUsage() 
{
	cout << "Sensors Guard usage:" << endl;
	cout << "SensorsGuard <command>" << endl;
	cout << "Commands:" << endl;
	cout << "\tstart - start SensorsGuard" << endl;
	cout << "\tstop [force] - stop SensorsGuard" << endl;
	cout << "\trestart [force] - restart SensorsGuard" << endl;
}

int findGuardProcess() 
{
	FILE* fp_output = popen( "ps a | grep \"SensorsGuard start\" | grep -v \"grep\"", "r" );
	if (fp_output == NULL) 
		return 0;

	int this_pid = getpid(), parent_pid = getppid();
	cout << "this pid : " << this_pid << ", parent pid : " << parent_pid << endl;
	int pid;

	while (!feof(fp_output)) {
		char psLine[200];
		if (fgets(psLine, 200, fp_output) == NULL)
			break;
		cout << "0 >" << psLine;

		sscanf( psLine, "%d", &pid );
		cout << "pid : " << pid << endl;

		if (pid != this_pid && pid != parent_pid)
			break;
		pid = 0;
		
		while (strlen(psLine) == 200) {
			fgets( psLine, 200, fp_output );
			cout << "1 >" << psLine;
		}
	}

	pclose( fp_output );
//	cout << "pid : " << pid << endl;
	return pid;
}

void waitForStopFile() {

	FILE* fp; 
	while (1) {
		fp = fopen( "./stopSensorsGuard", "r" );
		if (fp != NULL) {
			fclose( fp );
			break;
		}
		std::this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
}

void startGuard(int argc, char** argv) 
{
	syslog( LOG_NOTICE, "Sensors Guard starting." );

	int guardProcessId = findGuardProcess();
	if (guardProcessId > 0) {
		syslog( LOG_NOTICE, "The Sensors Guard already started, pid %d.", guardProcessId );

		cout << "Found SensorsGuard process (pid = " << guardProcessId << ")." << endl;
		cout << "Can not start the second SensorsGuard." << endl;
		return;
	}

	syslog( LOG_NOTICE, "Sensors Guard started." );
	
	SensorsGuardConfig config;
	std::ifstream inConfigFile( "sensors_guard.cfg" );
	if (inConfigFile.is_open()){
		config.deserialize( inConfigFile );
		inConfigFile.close();
	}
//	config.listSensors();
	
	stopFile.remove();
	
	SensorsGuard guard( &config );
	guard.start();

	stopFile.waitForNotExists( 1000 );
	guard.stop();
	
	cout << "Sensors Guard stopped." << endl;

	syslog( LOG_NOTICE, "Sensors Guard stopped." );
}

bool stopGuardSoft() 
{
	stopFile.create();
	
	int guardProcessId;
	for (int i = 0; i < 5; i++) {
		guardProcessId = findGuardProcess();
		if (guardProcessId <= 0) 
			break;
		std::this_thread::sleep_for( chrono::milliseconds( 1000 ) );
	}
	stopFile.remove();
	
	return guardProcessId <= 0;
}

bool stopGuardForce (int guardProcessId) 
{
	return kill( guardProcessId, 9 ) == 0;
}

bool doStop (int processId, bool force) 
{
	syslog( LOG_NOTICE, "The Sensors Guard process found, pid %d.", processId );
	cout << "Found SensorsGuard process (pid = " << processId << ")." << endl;
	
	bool result = false; 
	if (stopGuardSoft()) 
	{
		syslog( LOG_NOTICE, "The Sensors Guard process was stopped." );
		result = true; 
	}
	else 
	{
		if (force) 
		{
			syslog( LOG_NOTICE, "Killing of the Sensors Guard process." );
			if (stopGuardForce( processId ))
			{
				syslog( LOG_NOTICE, "The Sensors Guard process was killed." );
				result = true; 
			}
			else
				syslog( LOG_NOTICE, "The Sensors Guard process was not killed." );
		}
		else 
			syslog( LOG_NOTICE, "The Sensors Guard process was not stopped. Try the force option." );
	}
	return result;
}

void stopGuard(int argc, char** argv) 
{
	bool force = false;
	if (argc > 0) {
		string argv1( argv[0] );
		if (argv1.compare("force") == 0) {
			force = true;
		}
		else {
			cout << "SensorsGuard stop invalid option '" << argv1 << "')." << endl;
			printUsage();
			return;
		}
	}

	syslog( LOG_NOTICE, "Sensors Guard stopping." );

	int guardProcessId = findGuardProcess();
	if (guardProcessId > 0) 
	{
		doStop( guardProcessId, force );
	} 
	else {
		syslog( LOG_NOTICE, "The Sensors Guard process not found." );
	}
}

void restartGuard(int argc, char** argv) 
{
	bool force = false;
	if (argc > 0) {
		string argv1( argv[0] );
		if (argv1.compare("force") == 0) {
			force = true;
		}
		else {
			cout << "SensorsGuard restart invalid option '" << argv1 << "')." << endl;
			printUsage();
			return;
		}
	}

	syslog( LOG_NOTICE, "Sensors Guard restarting." );

	int guardProcessId = findGuardProcess();
	if (guardProcessId > 0) 
	{
		if (doStop( guardProcessId, force )) 
		{
			system( "./SensorsGuard start &" );
		}
		else {
			syslog( LOG_NOTICE, "The Sensors Guard was not restarted." );
		}
		
	} 
	else {
		syslog( LOG_NOTICE, "The Sensors Guard process not found." );
	}
}

int main(int argc, char** argv) 
{
	try
	{
		if (argc < 2) {
			printUsage();
		}
		else {
			string argv1( argv[1] );
			if (argv1.compare("start") == 0) {
				openlog( "SensorsGuard", LOG_PID, LOG_USER );
				startGuard( argc - 2, argv + 2 );
			} else 
			if (argv1.compare("stop") == 0) {
				openlog( "SensorsGuard stop", LOG_PID, LOG_USER );
				stopGuard( argc - 2, argv + 2 );
			} else 
			if (argv1.compare("restart") == 0) {
				openlog( "SensorsGuard restart", LOG_PID, LOG_USER );
				restartGuard( argc - 2, argv + 2 );
			}
		}
	}
	catch (exception& error) {
		cout << typeid( error ).name() << " - " << error.what() << endl;
	}

	closelog();
}
// ---------------------------------------------------------------------
	
