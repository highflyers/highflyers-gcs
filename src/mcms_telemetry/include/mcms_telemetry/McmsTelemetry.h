/*
 * McmsTelemetry.h
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MCMSTELEMETRY_H_
#define MCMSTELEMETRY_H_

#include "core/plugin_interfaces/ITelemetryControlsPlugin.h"

namespace Ui {
	class McmsTelemetry;
}

namespace HighFlyers {

class McmsTelemetry : public ITelemetryControlsPlugin, public QWidget
{
	Q_OBJECT

private:
	Ui::McmsTelemetry* ui;

public:
	McmsTelemetry( QWidget* parent = 0 );
	QWidget* get_widget() { return this; }
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
