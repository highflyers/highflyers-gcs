/*
 * IPlugin.h
 *
 *  Created on: 24 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef I_PLUGIN_H_
#define I_PLUGIN_H_

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
public:
	virtual ~IPlugin() {}
	static PluginType get_type() { return PluginType::UNKNOW; }
	virtual PluginType get_type_t() = 0;
};

}

#endif /* I_PLUGIN_H_ */
