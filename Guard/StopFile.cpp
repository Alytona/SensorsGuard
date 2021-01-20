# include <chrono>
# include <thread>

# include "StopFile.hpp"

using namespace std;

StopFile::StopFile( const char* filename ) {
	Filename = filename;
}

bool StopFile::create() {
	FILE* fp = fopen( Filename.c_str(), "w" ); 
	if (fp == NULL)
		return false;
	fclose( fp );
	return true;
}

bool StopFile::remove() {
	FILE* fp = fopen( Filename.c_str(), "r" );
	if (fp != NULL) {
		fclose( fp );
		return system( (string( "rm " ) + Filename).c_str() ) == 0;
	}
	return true;
}

void StopFile::waitForNotExists( int retryIntervalMS ) {
	FILE* fp;
	while ((fp = fopen( Filename.c_str(), "r" )) == NULL) 
		this_thread::sleep_for( chrono::milliseconds( retryIntervalMS ) );
	fclose( fp );
}
