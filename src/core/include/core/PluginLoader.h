/*
 * PluginLoader.h
 *
 *  Created on: 23 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINLOADER_H_
#define PLUGINLOADER_H_

#include "Singleton.h"
#include "plugin_interfaces/IPlugin.h"
#include "PluginObserver.h"
#include "IObservable.h"
#include <string>
#include <map>

namespace HighFlyers
{

class PluginLoader_  : public IObservable<PluginObserver>
{
private:
	std::map<std::string, void*> libraries;
	bool is_plugin_loaded( const std::string& filename );
	std::string get_last_error();

public:
	PluginLoader_();
	virtual ~PluginLoader_();

	void open_plugin( const std::string& filename );
	void close_plugin( const std::string& filename );

	IPlugin* get_object( const std::string& filename, PluginType type );

	template<typename T>
	T* get_object( const std::string& filename )
	{
		return static_cast<T*>( get_object( filename, T::get_type() ) );
	}
};

typedef Singleton<PluginLoader_> PluginLoader;
}
#endif /* PLUGINLOADER_H_ */
