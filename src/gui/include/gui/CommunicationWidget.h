/*
 * CommunicationWidget.h
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMUNICATIONWIDGET_H_
#define COMMUNICATIONWIDGET_H_

#include <QWidget>

namespace Ui {
class CommunicationWidget;
}

class CommunicationWidget : public QWidget
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
};


#endif /* COMMUNICATIONWIDGET_H_ */
