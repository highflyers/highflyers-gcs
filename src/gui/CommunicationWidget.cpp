/*
 * CommunicationWidget.cpp
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#include "ui_CommunicationWidget.h"
#include "gui/CommunicationWidget.h"

CommunicationWidget::CommunicationWidget( QWidget *parent )
: QWidget( parent ),
  ui( new Ui::CommunicationWidget )
{
	ui->setupUi( this );
}

CommunicationWidget::~CommunicationWidget()
{
	delete ui;
}

void CommunicationWidget::set_state( bool opened )
{
	ui->openClosePushButton->setText( opened ? "Close" : "Open" );
}

void CommunicationWidget::blink_receive()
{
	// TODO blinking during milliseconds instead of infinity;)
	ui->readingStateRadioButton->setChecked( true );
}

void CommunicationWidget::blink_send()
{
	ui->writingStateRadioButton->setChecked( true );
}
