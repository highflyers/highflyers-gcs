/*
 * MultiOutPolicy.h
 *
 *  Created on: Dec 16, 2014
 *      Author: loganek
 */

#ifndef MULTIOUTPOLICY_H_
#define MULTIOUTPOLICY_H_

#include <iostream>
#include <fstream>

namespace HighFlyers {

class MultiOutPolicy
{
	template<typename T>
	friend MultiOutPolicy & operator<< ( MultiOutPolicy& output, const T &s );
	friend HighFlyers::MultiOutPolicy& operator<< ( HighFlyers::MultiOutPolicy& os, std::ostream& (*f)( std::ostream& ) );

	std::ofstream filestream;
	std::string filename;
public:
	void init( const std::string& filename );
	void init();

	std::string get_filename() const { return filename; }
};

template<typename T>
MultiOutPolicy & operator<< ( MultiOutPolicy& output, const T &s )
{
	std::cout << s;
	output.filestream << s;
	return output;
}

}

#endif /* MULTIOUTPOLICY_H_ */
