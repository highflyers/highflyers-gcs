/*
 * VideoTestSrc.h
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef VIDEOTESTSRC_H_
#define VIDEOTESTSRC_H_

#include "core/plugin_interfaces/IVideoSourcePlugin.h"

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/videooverlay.h>
#include <QtWidgets>

namespace HighFlyers
{

class VideoTestSrc : public IVideoSourcePlugin
{
private:
	GstElement* source;
	GstElement* converter;
	GstElement* window_sink;
	GstElement* tee;
	GstElement* queue;
	GstElement* pipeline;
	GstElement* app_sink;
	unsigned int window_handler;
	Image* current_image;

private:
	void save_params( const std::string& pattern );

public:
	VideoTestSrc();
	virtual ~VideoTestSrc();

	virtual void play( bool recording );
	virtual void stop();
	virtual Image* get_image();
	virtual void set_render_window( unsigned int handler );
	virtual void set_filename( const std::string& filename );
	virtual QWidget* get_config_window();

	static GstFlowReturn new_appsink_sample( GstAppSink *gstappsink, gpointer object );
};

}

#endif /* VIDEOTESTSRC_H_ */
