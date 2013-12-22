/*
 * ITelemetryControlsPlugin.h
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef ITELEMETRYCONTROLSPLUGIN_H_
#define ITELEMETRYCONTROLSPLUGIN_H_

#include "IPlugin.h"
#include <QWidget>

namespace HighFlyers {

class ITelemetryControlsPlugin : public IPlugin
{
public:
	virtual QWidget* get_widget() = 0;
	virtual PluginType get_type_t() { return PluginType::TELEMETRY; }
	virtual ~ITelemetryControlsPlugin() {}

	virtual void set_battery_value( double value ) = 0;
	virtual void set_latitude( double value ) = 0;
	virtual void set_longitude( double value ) = 0;
	virtual void set_heading( double value ) = 0;
	virtual void set_yaw( double value ) = 0;
	virtual void set_roll( double value ) = 0;
	virtual void set_pitch( double value ) = 0;
};

}


#endif /* ITELEMETRYCONTROLSPLUGIN_H_ */
