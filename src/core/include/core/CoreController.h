/*
 * CoreController.h
 *
 *  Created on: 15 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef CORECONTROLLER_H_
#define CORECONTROLLER_H_

#include "PluginLoader.h"
#include "PluginObserver.h"
#include "gui/MainWindow.h"
#include <functional>
#include <vector>

namespace HighFlyers
{
class CoreController :  public PluginObserver
{
private:
	PluginLoader_* loader;
	MainWindow* gui;

	std::vector<IPlugin*> get_plugins( std::function<bool(IPlugin* plugin)> cond ) const;
	void reg_unreg_plugin( IPlugin* plugin, PluginType type, PluginSuperPower super_power, bool register_plugin );

protected:
	virtual void plugin_loaded( IObservable<PluginObserver>* sender, IPlugin* plugin );
	virtual void plugin_unloaded( IObservable<PluginObserver>* sender, std::string filename );

public:
	CoreController( MainWindow* gui );
	virtual ~CoreController(){}

	void load_set_of_plugins( const std::string& filename );
	void load_plugin( const std::string& filename );
	void unload_plugin( const std::string& filename );
};

}
#endif /* CORECONTROLLER_H_ */
