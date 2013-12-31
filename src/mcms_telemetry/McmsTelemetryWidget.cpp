/*
 * McmsTelemetryWidget.cpp
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#include "mcms_telemetry/McmsTelemetryWidget.h"
#include "ui_McmsTelemetry.h"

McmsTelemetryWidget::McmsTelemetryWidget( QWidget* parent )
: QWidget( parent ),
  ui(new Ui::McmsTelemetry)
{
	ui->setupUi(this);
}

