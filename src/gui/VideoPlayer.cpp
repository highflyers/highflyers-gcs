/*
 * VideoPlayer.cpp
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#include "gui/VideoPlayer.h"

VideoPlayer::VideoPlayer( QWidget* parent )
: QWidget( parent ),
  image_canvas(new QFrame())
{
	setLayout( new QVBoxLayout() );
	layout()->addWidget( image_canvas );

	QHBoxLayout* btn_layout = new QHBoxLayout();
	QPushButton* button = new QPushButton( "Play" );
	btn_layout->addWidget( button );
	connect(button, &QPushButton::clicked, [this]{
		Q_EMIT state_clicked( true );
	} );

	button = new QPushButton( "Stop" );
	btn_layout->addWidget( button );
	connect(button, &QPushButton::clicked, [this]{
		Q_EMIT state_clicked( false );
	} );

	button = new QPushButton( "Configuration" );
	btn_layout->addWidget( button );
	connect(button, &QPushButton::clicked, [this]{
		Q_EMIT config_window_requested();
	} );

	QFrame* btn_frame = new QFrame();
	btn_frame->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum );
	btn_frame->setLayout( btn_layout );
	layout()->addWidget( btn_frame );
}

WId VideoPlayer::get_canvas_handler()
{
	return image_canvas->winId();
}
