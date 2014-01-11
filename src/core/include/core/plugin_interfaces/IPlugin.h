/*
 * IPlugin.h
 *
 *  Created on: 24 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef I_PLUGIN_H_
#define I_PLUGIN_H_

#include <string>

namespace HighFlyers
{
enum class PluginType
{
	UNKNOW,
	APCONFIG,
	TELEMETRY,
	VIDEO,
	COMMUNICATION,
};

class IPlugin
{
protected:
	std::string plugin_location;
	void* library;

public:
	virtual ~IPlugin() {}
	static PluginType get_type() { return PluginType::UNKNOW; }
	virtual PluginType get_type_t() = 0;

	void set_plugin_location( const std::string& plugin_location )
	{
		this->plugin_location = plugin_location;
	}

	std::string get_plugin_location() const
	{
		return plugin_location;
	}
};

}

#endif /* I_PLUGIN_H_ */
