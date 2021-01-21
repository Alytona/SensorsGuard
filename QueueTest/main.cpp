
# include <iostream>
# include <fstream>
# include <string>
# include <typeinfo>
# include <syslog.h>
# include <chrono>
# include <thread>
# include <signal.h>
# include <cstdio>

# include "SensorQueue.hpp"
# include "MessagesProducer.hpp"
# include "MessagesReader.hpp"

using namespace std;

int main(int argc, char** argv) 
{
	try
	{
		SensorQueue queue;
		MessagesProducer producer( &queue );
		MessagesReader reader( &queue );
		
		producer.start();
		reader.start();

		char ch;
		getchar();
		cout << "Stop." << endl; 
		
		producer.stop();
		reader.stop();
	}
	catch (exception& error) {
		cout << typeid( error ).name() << " - " << error.what() << endl;
	}
}
// ---------------------------------------------------------------------
	
