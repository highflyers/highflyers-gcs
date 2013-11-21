/*
 * CoreController.cpp
 *
 *  Created on: 15 lis 2013
 *      Author: Marcin Kolny
 */

#include "CoreController.h"
#include "plugin_interfaces.h"

using namespace HighFlyers;
using namespace std;

CoreController::CoreController( MainWindow* gui )
: gui(gui)
{
	loader = PluginLoader::get_instance();
	loader->register_observer( this );
	gui->set_controller( this );
}

void CoreController::load_plugin( const string& filename )
{
	try
	{
		loader->open_plugin( filename );
	}
	catch (const std::exception& ex)
	{
		loader->close_plugin( filename );
		throw ex;
	}
}

void CoreController::unload_plugin( const string& filename )
{
	loader->close_plugin( filename );
}

void CoreController::plugin_loaded( IObservable<PluginObserver>* sender, std::string filename )
{
	gui->plugin_added( loader->get_object( filename, PluginType::UNKNOW ), filename.c_str() );
}

void CoreController::plugin_unloaded( IObservable<PluginObserver>* sender, std::string filename )
{
	gui->plugin_removed( filename.c_str() );
}