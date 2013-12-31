/*
 * McmsTelemetry.h
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MCMSTELEMETRY_H_
#define MCMSTELEMETRY_H_

#include "core/plugin_interfaces/ITelemetryControlsPlugin.h"
#include "mcms_telemetry/McmsTelemetryWidget.h"

class McmsTelemetry : public HighFlyers::ITelemetryControlsPlugin
{
	McmsTelemetryWidget* tw;

public:
	McmsTelemetry();
	virtual ~McmsTelemetry();
	QWidget* get_widget() { return tw; }
	void set_battery_value( double value );
	void set_latitude( double value );
	void set_longitude( double value );
	void set_heading( double value );
	void set_yaw( double value );
	void set_roll( double value );
	void set_pitch( double value );
};

#endif /* MCMSTELEMETRY_H_ */
