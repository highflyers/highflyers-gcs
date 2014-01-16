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

enum class PluginSuperPower
{
	NO_SUPER_POWER = 0,
	DATA_RECEIVER = 1,
	DATA_SENDER = 2,
	FRAME_RECEIVER = 4,
	FRAME_SENDER = 8
};

template<typename T>
T operator|(T l, T r)
{
	return static_cast<T>(static_cast<int>(l)|static_cast<int>(r));
}

template<typename T>
T operator&(T l, T r)
{
	return static_cast<T>(static_cast<int>(l)&static_cast<int>(r));
}

class IPlugin
{
protected:
	std::string plugin_location;

public:
	virtual ~IPlugin() {}
	static PluginType get_type() { return PluginType::UNKNOW; }
	virtual PluginType get_type_t() = 0;
	virtual PluginSuperPower get_super_power() const { return PluginSuperPower::NO_SUPER_POWER; }
	virtual bool has_super_power( PluginSuperPower super_power ) const
	{ return (get_super_power() & super_power) != PluginSuperPower::NO_SUPER_POWER; }

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
