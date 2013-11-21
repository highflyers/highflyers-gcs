/*
 * PluginObserver.h
 *
 *  Created on: 19 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINOBSERVER_H_
#define PLUGINOBSERVER_H_

#include "IObservable.h"

namespace HighFlyers
{
class PluginObserver
{
public:
	virtual ~PluginObserver(){}
	virtual void plugin_loaded( IObservable<PluginObserver>* sender, std::string filename ) = 0;
	virtual void plugin_unloaded( IObservable<PluginObserver>* sender, std::string filename ) = 0;
};
}

#endif /* PLUGINOBSERVER_H_ */
