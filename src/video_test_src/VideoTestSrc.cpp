/*
 * VideoTestSrc.cpp
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#include "video_test_src/VideoTestSrc.h"

using namespace HighFlyers;

VideoTestSrc::VideoTestSrc()
{
	gst_init( NULL, NULL );
	source = gst_element_factory_make( "videotestsrc", NULL );
	window_sink = gst_element_factory_make( "xvimagesink", NULL );
	tee = gst_element_factory_make( "tee", NULL );
	queue = gst_element_factory_make( "queue", NULL );
	app_sink = gst_element_factory_make( "appsink", NULL );
	pipeline = gst_pipeline_new( NULL );

	gst_bin_add_many( GST_BIN( pipeline ), source,
			tee, queue, window_sink, app_sink, NULL );

	gst_element_link_many( source, tee, queue, app_sink, NULL );
	gst_element_link_many( tee, window_sink, NULL );
}

VideoTestSrc::~VideoTestSrc()
{
	stop();

	gst_object_unref( GST_OBJECT( source ) );
	gst_object_unref( GST_OBJECT( pipeline ) );
	gst_object_unref( GST_OBJECT( window_sink ) );
	gst_object_unref( GST_OBJECT( tee ) );
	gst_object_unref( GST_OBJECT( queue) );
	gst_object_unref( GST_OBJECT( app_sink ) );
}

void VideoTestSrc::play( bool recording )
{
	// no recording implemented, it's test only.
	gst_element_set_state( pipeline, GST_STATE_PLAYING );
}

void VideoTestSrc::stop()
{
	gst_element_set_state( pipeline, GST_STATE_NULL );
}

Image* VideoTestSrc::get_image()
{
	return nullptr;
}

void VideoTestSrc::set_render_window( unsigned int handler )
{
	gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( window_sink ), handler );
}

void VideoTestSrc::set_filename( const std::string& filename )
{
	GParamSpec* param_spec =
			g_object_class_find_property( G_OBJECT_GET_CLASS( source ), "pattern" );

	int pos = 0;

	GEnumValue *values;
	guint j = 0;
	values = G_ENUM_CLASS (g_type_class_ref(param_spec->value_type))->values;

	while (values[j].value_name)
	{
		if (filename == values[j].value_nick)
		{
			pos = values[j].value;
			break;
		}
		j++;
	}

	g_object_set( G_OBJECT( source ), "pattern", pos );
}
