/*
 * CoreController.cpp
 *
 *  Created on: 15 lis 2013
 *      Author: Marcin Kolny
 */

#include "CoreController.h"
#include "apconfig/ApConfig.h"

using namespace HighFlyers;
using namespace std;

CoreController::CoreController()
{
	loader = PluginLoader::get_instance();
}

IPluginInterface* CoreController::load_plugin(const string& filename, PluginType type)
{
	try
	{
		loader->open_plugin(filename);

		switch (type)
		{
		case PluginType::APCONFIG:
			return loader->get_object<ApConfig>(filename);
		case PluginType::UNKNOW:
			return loader->get_object(filename, type);
		}
	}
	catch (const std::exception& ex)
	{
		loader->close_plugin(filename);
		throw ex;
	}
}


