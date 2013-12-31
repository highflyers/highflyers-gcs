/*
 * McmsTelemetry.cpp
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#include "ui_McmsTelemetry.h"
#include "mcms_telemetry/McmsTelemetry.h"

McmsTelemetry::McmsTelemetry()
{
	tw = new McmsTelemetryWidget();
}

McmsTelemetry::~McmsTelemetry()
{
	delete tw;
}

#define SETTER_IMPL(WHAT, QT_CONTROL) \
void McmsTelemetry::set_##WHAT( double value ) \
{ \
	tw->get_ui()->QT_CONTROL->setText(QString::number(value)); \
}

SETTER_IMPL(battery_value, batteryVoltageLineEdit)
SETTER_IMPL(latitude, latitudeLineEdit)
SETTER_IMPL(longitude, longitudeLineEdit)
SETTER_IMPL(heading, headingLineEdit)
SETTER_IMPL(yaw, yawLineEdit)
SETTER_IMPL(pitch, pitchLineEdit)
SETTER_IMPL(roll, rollLineEdit)

