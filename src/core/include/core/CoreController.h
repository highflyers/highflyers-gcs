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

#include <vector>

namespace HighFlyers
{
class CoreController :  public PluginObserver
{
private:
	PluginLoader_* loader;
	MainWindow* gui;

	std::vector<IPlugin*> get_plugins_with_super_power( PluginSuperPower super_power ) const;

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
