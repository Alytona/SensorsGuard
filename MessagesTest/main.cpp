
# include <iostream>
# include <fstream>
# include <string>
# include <typeinfo>
# include <syslog.h>
# include <chrono>
# include <thread>
# include <signal.h>

using namespace std;

int getMessage( int messages[], int size, int index ) 
{
	if (index >= size) 
		return -1;
		
	return messages[index];
}

void testMessages( int messages[], int size ) 
{
	for (int i = 0; i < size; i++) 
		cout << messages[i] << " ";
	cout << endl;

	int i = 0;
	int message = -1;
	int nextMessage = getMessage( messages, size, i++ );
	while (nextMessage != -1) 
	{
		if (message != -1 && nextMessage - message > 3) {
			cout << "0: " << message << endl;
		}
		message = nextMessage;
		nextMessage = getMessage( messages, size, i++ );
	}
	if (message != -1)
	{
		cout << "1: " << message << endl;
	}
}

int main(int argc, char** argv) 
{

	try
	{
		int messages6[] = { 0, 1, 4, 5, 7, 9 };
		testMessages( messages6, 6 );
		cout << endl;

		int messages1[] = { 0, 4 };
		testMessages( messages1, 2 );
		cout << endl;

		int messages2[] = { 0, 3, 7, 8 };
		testMessages( messages2, 4 );
		cout << endl;
	}
	catch (exception& error) {
		cout << typeid( error ).name() << " - " << error.what() << endl;
	}
}
// ---------------------------------------------------------------------
	
