/*
 * MultiOutPolicy.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: loganek
 */

#include "logger/MultiOutPolicy.h"

#include <ctime>
#include <cstdio>

#ifdef __linux__
#include <unistd.h>
#define GetCurrentDir getcwd
#elif defined _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
	static_assert(false, "Unsupported OS.");
#endif

namespace HighFlyers {

void MultiOutPolicy::init( const std::string& filename )
{
	filestream.open( filename );
	this->filename = filename;
}

void MultiOutPolicy::init()
{
	char current_path[FILENAME_MAX] = {0};
	std::string path;

	if (GetCurrentDir( current_path, sizeof( current_path ) ) != nullptr)
		path = current_path + std::string( "/" );

	std::string filename = path + "log-" + std::to_string( time( nullptr ) ) + ".txt";
	init( filename );
}


MultiOutPolicy& operator<< ( MultiOutPolicy& os, std::ostream& (*f)( std::ostream& ) )
{
	return operator<< <std::ostream& (*)( std::ostream& )> ( os, f );
}

}
