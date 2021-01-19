# pragma once

# include <string>
# include <chrono>
# include <thread>

using namespace std;

class StopFile {

	string Filename;
	
public :	
	StopFile( const char* filename ) {
		Filename = filename;
	}
	
	bool create() {
		FILE* fp = fopen( Filename.c_str(), "w" ); 
		if (fp == NULL)
			return false;
		fclose( fp );
		return true;
	}

	bool remove() {
		FILE* fp = fopen( Filename.c_str(), "r" );
		if (fp != NULL) {
			fclose( fp );
			return system( (string( "rm " ) + Filename).c_str() ) == 0;
		}
		return true;
	}
	
	void waitForNotExists( int retryIntervalMS ) {
		FILE* fp;
		while ((fp = fopen( Filename.c_str(), "r" )) == NULL) 
			this_thread::sleep_for( chrono::milliseconds( retryIntervalMS ) );
		fclose( fp );
	}
};
