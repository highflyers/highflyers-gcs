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

public:
	virtual ~VideoPlayer() {}
	VideoPlayer( QWidget* parent = 0 );
};

#endif /* VIDEOPLAYER_H_ */
