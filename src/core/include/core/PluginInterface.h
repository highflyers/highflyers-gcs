/*
 * PluginInterface.h
 *
 *  Created on: 24 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGININTERFACE_H_
#define PLUGININTERFACE_H_

enum class PluginType
{
	UNKNOW,
	APCONFIG,
	TELEMETRY,
};

class IPluginInterface
{
public:
	virtual ~IPluginInterface() {}
	static PluginType get_type() { return PluginType::UNKNOW; }
};

#endif /* PLUGININTERFACE_H_ */
