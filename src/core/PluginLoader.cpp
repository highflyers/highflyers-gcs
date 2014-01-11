/*
 * PluginLoader.cpp
 *
 *  Created on: 23 pa? 2013
 *      Author: Marcin Kolny
 */

#include "core/PluginLoader.h"
#ifdef __linux__
#include <dlfcn.h>
#elif defined _WIN32
#include <windows.h>
#else
	static_assert(false, "Unsupported OS.");
#endif
#include <stdexcept>

using namespace HighFlyers;

PluginLoader_::PluginLoader_()
{
}

PluginLoader_::~PluginLoader_()
{
	for (auto& plugin : plugins)
	{
		try
		{
			close_plugin(plugin.first);
		}
		catch (const std::runtime_error&)
		{
			// TODO how to report it?
		}
	}
}

bool PluginLoader_::is_plugin_loaded(const std::string& filename)
{
	return plugins.count(filename) && plugins[filename].library != nullptr;
}

void PluginLoader_::load_plugin(const std::string& filename)
{
	if (is_plugin_loaded(filename))
		return;

	void* lib =
#ifdef __linux__
			dlopen(filename.c_str(), RTLD_NOW);
#elif defined _WIN32
			reinterpret_cast<void*>(LoadLibrary(filename.c_str()));
#else
			nullptr; static_assert(false, "Unsupported OS.");
#endif

	if (lib == nullptr)
		throw std::runtime_error("Cannot load plugin " +
				filename + ". Error: " + get_last_error());

	plugins[filename].library = lib;
	plugins[filename].plugin = get_object(filename, PluginType::UNKNOW);
	notify<IPlugin*>(&PluginObserver::plugin_loaded, plugins[filename].plugin);
}

void PluginLoader_::close_plugin(const std::string& filename)
{
	if (!is_plugin_loaded(filename))
		return;

	delete plugins[filename].plugin;

	int ret =
#ifdef __linux__
			dlclose(plugins[filename].library);
#elif defined _WIN32
			FreeLibrary(reinterpret_cast<HMODULE>(libraries[filename]));
#else
			0; static_assert(false, "Unsupported OS.");
#endif

	if (ret)
		throw std::runtime_error("Cannot close plugin " +
				filename + ". Error: " + get_last_error());

	plugins.erase(filename);
	notify<std::string>(&PluginObserver::plugin_unloaded, filename);
}

std::string PluginLoader_::get_last_error()
{
	return 
#ifdef __linux__
		dlerror();
#else
		"unknow error";
#endif
}

IPlugin* PluginLoader_::get_object( const std::string& filename, PluginType type )
{
	if (!is_plugin_loaded( filename ))
		throw std::runtime_error( "Plugin " + filename + " not loaded." );

	IPlugin* iface;

	void* function_symbol =
#ifdef __linux__
		dlsym( plugins[filename].library, "factory_method" );
#elif defined _WIN32
		(FactoryMethod)(::GetProcAddress( reinterpret_cast<HMODULE>( libraries[filename] ),
				"factory_method" ) );
#else
		nullptr; static_assert( false, "Unsupported OS" );
#endif

	if (function_symbol == nullptr)
		throw std::runtime_error( "Cannot load factory method: " + get_last_error() );

	typedef IPlugin* (*FactoryMethod)();
	FactoryMethod fm;

	*reinterpret_cast<void **>(&fm) = function_symbol;
	iface = fm();

	if (iface == nullptr)
		throw std::runtime_error( "Cannot create object." );

	if (type != PluginType::UNKNOW && iface->get_type_t() != type)
		throw std::runtime_error( "Cannot get specific object. Invalid plugin type." );

	iface->set_plugin_location(filename);

	return iface;
}

std::map<std::string, PluginLoader_::PluginLibrary> PluginLoader_::get_plugins() const
{
	return plugins;
}
