/*
 * PluginSetLoader.h
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINSETLOADER_H_
#define PLUGINSETLOADER_H_

#include <set>
#include <string>

class PluginSetLoader
{
	std::string configuration_file;
	std::set<std::string> filenames;

	void load_filenames();

public:
	void load(const std::string& configuration_file);
	std::set<std::string> get_filenames() const;
};

#endif /* PLUGINSETLOADER_H_ */
