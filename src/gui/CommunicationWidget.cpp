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

	connect(ui->openClosePushButton, &QPushButton::clicked, [this](bool){
		Q_EMIT open_close_clicked(ui->openClosePushButton->text() == "Open"); // fixme ugly code :/
	});
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

void CommunicationWidget::on_receive( HighFlyers::IObservable<HighFlyers::CommunicationObserver>* sender, std::string data )
{
	blink_receive();
}

void CommunicationWidget::on_state_changed( HighFlyers::IObservable<HighFlyers::CommunicationObserver>* sender, bool state )
{
	set_state(state);
}
