/*
 * McmsTelemetry.h
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MCMSTELEMETRY_H_
#define MCMSTELEMETRY_H_

#include "core/plugin_interfaces/ITelemetryControlsPlugin.h"

namespace HighFlyers {

class McmsTelemetry : public ITelemetryControlsPlugin
{
	QWidget* get_widget() {return nullptr;}
	void set_battery_value( double value ){}
	void set_latitude( double value ){}
	void set_longitude( double value ){}
	void set_heading( double value ){}
	void set_yaw( double value ){}
	void set_roll( double value ){}
	void set_pitch( double value ){}
};

}

#endif /* MCMSTELEMETRY_H_ */
