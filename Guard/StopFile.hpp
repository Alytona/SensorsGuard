# pragma once

# include <string>

class StopFile {

	std::string Filename;
	
public :	
	StopFile( const char* filename );

	bool create();
	bool remove();
	
	void waitForNotExists( int retryIntervalMS );
};
