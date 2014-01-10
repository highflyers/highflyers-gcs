/*
 * CommunicationWidget.h
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMUNICATIONWIDGET_H_
#define COMMUNICATIONWIDGET_H_

#include "core/plugin_interfaces/ICommunicationPlugin.h"
#include <QWidget>

namespace Ui {
class CommunicationWidget;
}

class CommunicationWidget : public QWidget, public HighFlyers::CommunicationObserver
{
	Q_OBJECT

private:
	Ui::CommunicationWidget* ui;

public:
	explicit CommunicationWidget( QWidget *parent = 0 );
	~CommunicationWidget();

	void set_state( bool opened );
	void blink_receive();
	void blink_send();

	void on_receive( HighFlyers::IObservable<HighFlyers::CommunicationObserver>* sender, std::string data );
	void on_state_changed( HighFlyers::IObservable<HighFlyers::CommunicationObserver>* sender, bool state );

Q_SIGNALS:
	void open_close_clicked( bool state );
};


#endif /* COMMUNICATIONWIDGET_H_ */
