/*
 * PluginSetLoader.cpp
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginSetLoader.h"
#include "PluginLoader.h"
#include <fstream>
#include <stdexcept>

void PluginSetLoader::load_filenames()
{
	std::ifstream file(configuration_file);

	if (!file.is_open())
		throw std::runtime_error("cannot open configuration file " + configuration_file);

	filenames.clear();

	while (!file.eof())
	{
		std::string line;
		file >> line;

		if (!line.empty())
			filenames.insert(line);
	}
}

void PluginSetLoader::load(const std::string& configuration_file)
{
	this->configuration_file = configuration_file;
	load_filenames();

	for (auto plugin_filename : filenames)
	{
		if (plugin_filename.size() > 0 && plugin_filename[0] != '#')
		HighFlyers::PluginLoader::get_instance()->open_plugin(plugin_filename);
	}
}
