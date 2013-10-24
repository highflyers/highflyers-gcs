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
	APCONFIG,
	TELEMETRY,
};

class IPluginInterface
{
public:
	PluginType get_type() = 0;
};

#endif /* PLUGININTERFACE_H_ */
