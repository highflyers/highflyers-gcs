/*
 * CoreController.cpp
 *
 *  Created on: 15 lis 2013
 *      Author: Marcin Kolny
 */

#include "CoreController.h"
#include "plugin_interfaces.h"
#include "PluginSetLoader.h"
#include <stdexcept>

using namespace HighFlyers;
using namespace std;

CoreController::CoreController( MainWindow* gui )
: gui(gui)
{
	loader = PluginLoader::get_instance();
	loader->register_observer( this );
	gui->set_controller( this );
}

void CoreController::load_set_of_plugins( const string& filename )
{
	PluginSetLoader().load(filename);
}

void CoreController::load_plugin( const string& filename )
{
	try
	{
		loader->load_plugin( filename );
	}
	catch (const std::runtime_error& ex)
	{
		loader->close_plugin( filename );
		throw ex;
	}
}

void CoreController::reg_unreg_plugin( IPlugin* plugin, PluginType type, PluginSuperPower super_power, bool register_plugin )
{
	if (plugin->has_super_power(super_power))
	{
		auto f = (register_plugin) ?
				[](ICommunicationPlugin* emiter, CommunicationObserver* observer) {emiter->register_observer(observer);} :
				[](ICommunicationPlugin* emiter, CommunicationObserver* observer) {emiter->unregister_observer(observer);};

		vector<IPlugin*> emiters = get_plugins( [type](IPlugin* plugin) {
			return plugin->get_type_t() == type;
		});

		for (auto emiter : emiters)
			f( static_cast<ICommunicationPlugin*>( emiter ), ( dynamic_cast<CommunicationObserver*>( plugin ) ) );
	}
}

void CoreController::unload_plugin( const string& filename )
{
	reg_unreg_plugin( loader->get_plugins()[filename].plugin,
			PluginType::COMMUNICATION, PluginSuperPower::DATA_RECEIVER, false );
	loader->close_plugin( filename );
}

void CoreController::plugin_loaded( IObservable<PluginObserver>* sender, IPlugin* plugin )
{
	switch (plugin->get_type_t())
	{
	case PluginType::COMMUNICATION:
	{
		vector<IPlugin*> receivers = get_plugins( [](IPlugin* plugin) {
			return (plugin->get_super_power() & PluginSuperPower::DATA_RECEIVER) != PluginSuperPower::NO_SUPER_POWER;
		});

		for (auto rec : receivers)
			static_cast<ICommunicationPlugin*>( plugin )->register_observer( dynamic_cast<CommunicationObserver*>( rec ) );

		break;
	}
	default:
		break;
	}

	reg_unreg_plugin( plugin, PluginType::COMMUNICATION, PluginSuperPower::DATA_RECEIVER, true );
	gui->plugin_added( plugin );
}

void CoreController::plugin_unloaded( IObservable<PluginObserver>* sender, std::string filename )
{
	gui->plugin_removed( filename.c_str() );
}

vector<IPlugin*> CoreController::get_plugins( function<bool(IPlugin* p1ugin)> cond ) const
{
	vector<IPlugin*> plugins;

	auto plugin_libs = loader->get_plugins();

	for (auto pl : plugin_libs)
	{
		if (cond(pl.second.plugin))
			plugins.push_back(pl.second.plugin);
	}

	return plugins;
}
