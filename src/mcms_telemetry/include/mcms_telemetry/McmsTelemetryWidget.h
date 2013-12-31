/*
 * McmsTelemetryWidget.h
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MCMSTELEMETRYWIDGET_H_
#define MCMSTELEMETRYWIDGET_H_

#include <QWidget>

namespace Ui {
	class McmsTelemetry;
}

class McmsTelemetryWidget : public QWidget
{
	Q_OBJECT

private:
	Ui::McmsTelemetry* ui;

public:
	McmsTelemetryWidget( QWidget* parent = 0 );
	Ui::McmsTelemetry* get_ui(){ return ui; }
};

#endif /* MCMSTELEMETRYWIDGET_H_ */
