/*
 * VideoPlayer.h
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef VIDEOPLAYER_H_
#define VIDEOPLAYER_H_

#include <QtWidgets>

class VideoPlayer : public QWidget
{
	Q_OBJECT

private:
	QFrame* image_canvas;

public:
	virtual ~VideoPlayer() {}
	VideoPlayer( QWidget* parent = 0 );

	WId get_canvas_handler();

Q_SIGNALS:
	void state_clicked( bool play );
	void config_window_requested();
};

#endif /* VIDEOPLAYER_H_ */
