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

_PluginLoader::_PluginLoader()
{
}

_PluginLoader::~_PluginLoader()
{
	for (auto it = libraries.begin(); it != libraries.end(); ++it)
	{
		try
		{
			close_plugin(it->first);
		}
		catch (const std::runtime_error&)
		{}
	}
}

bool _PluginLoader::is_plugin_loaded(const std::string& filename)
{
	return !libraries.count(filename) || libraries[filename] != nullptr;
}

void _PluginLoader::open_plugin(const std::string& filename)
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

	libraries[filename] = lib;
}

void _PluginLoader::close_plugin(const std::string& filename)
{
	if (!is_plugin_loaded(filename))
		return;

	int ret =
#ifdef __linux__
			dlclose(libraries[filename]);
#elif defined _WIN32
			FreeLibrary(reinterpret_cast<HMODULE>(libraries[filename]));
#else
			0; static_assert(false, "Unsupported OS.");
#endif

	if (!ret)
		throw std::runtime_error("Cannot close plugin " +
				filename + ". Error: " + get_last_error());

	libraries.erase(filename);
}

std::string _PluginLoader::get_last_error()
{
	return dlerror();
}
