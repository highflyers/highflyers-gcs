/*
 * VideoTestSrc.cpp
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#include "video_test_src/VideoTestSrc.h"
#include <gst/video/video.h>

using namespace HighFlyers;

VideoTestSrc::VideoTestSrc()
: window_handler( -1 ),
  current_image( nullptr )
{
	gst_init( NULL, NULL );
	source = gst_element_factory_make( "videotestsrc", NULL );
	converter = gst_element_factory_make( "videoconvert", NULL );
	window_sink = gst_element_factory_make( "xvimagesink", NULL );
	tee = gst_element_factory_make( "tee", NULL );
	queue = gst_element_factory_make( "queue", NULL );
	app_sink = gst_element_factory_make( "appsink", NULL );
	pipeline = gst_pipeline_new( NULL );

	g_object_set( G_OBJECT(app_sink), "emit-signals", TRUE, NULL );

	gst_bin_add_many( GST_BIN( pipeline ), source, converter,
			tee, queue, window_sink, app_sink, NULL );

	gst_element_link_filtered( source, converter, gst_caps_from_string("video/x-raw,format=RGB") );
	gst_element_link_many( converter, tee, queue, app_sink, NULL );
	gst_element_link_many( tee, window_sink, NULL );

	g_signal_connect( app_sink, "new-sample", G_CALLBACK( VideoTestSrc::new_appsink_sample ), this );
}

VideoTestSrc::~VideoTestSrc()
{
	stop();

	gst_object_unref( GST_OBJECT( source ) );
	gst_object_unref( GST_OBJECT( converter ) );
	gst_object_unref( GST_OBJECT( pipeline ) );
	gst_object_unref( GST_OBJECT( window_sink ) );
	gst_object_unref( GST_OBJECT( tee ) );
	gst_object_unref( GST_OBJECT( queue) );
	gst_object_unref( GST_OBJECT( app_sink ) );

	delete current_image;
}

void VideoTestSrc::play( bool recording )
{
	// no recording implemented, it's test only.
	if (window_handler != -1)
		set_render_window( window_handler );
	gst_element_set_state( pipeline, GST_STATE_PLAYING );
}

void VideoTestSrc::stop()
{
	gst_element_set_state( pipeline, GST_STATE_NULL );
}

Image* VideoTestSrc::get_image()
{
	return current_image;
}

void VideoTestSrc::set_render_window( unsigned int handler )
{
	window_handler = handler;
	gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( window_sink ), handler );
}

void VideoTestSrc::set_filename( const std::string& filename )
{
	// not implemented
}

void VideoTestSrc::save_params( const std::string& pattern )
{
	GParamSpec* param_spec =
			g_object_class_find_property( G_OBJECT_GET_CLASS( source ), "pattern" );

	int pos = 0;

	GEnumValue *values;
	guint j = 0;
	values = G_ENUM_CLASS ( g_type_class_ref( param_spec->value_type ) )->values;

	while (values[j].value_name)
	{
		if (pattern == values[j].value_nick)
		{
			pos = values[j].value;
			break;
		}
		j++;
	}

	g_object_set( G_OBJECT( source ), "pattern", pos, NULL );
}

QWidget* VideoTestSrc::get_config_window()
{
	auto frame = new QFrame();

	frame->setLayout( new QVBoxLayout() );

	auto pattern_frame = new QFrame();
	frame->layout()->addWidget( pattern_frame );
	pattern_frame->setLayout( new QHBoxLayout() );
	pattern_frame->layout()->addWidget( new QLabel( "Source pattern: " ) );
	auto pattern_combo = new QComboBox();
	pattern_combo->addItems( { "smpte", "black", "red", "circular", "ball" } );
	pattern_frame->layout()->addWidget( pattern_combo );

	auto ok_canc_frame = new QFrame();
	ok_canc_frame->setLayout( new QHBoxLayout() );

	auto ok_canc_button = new QPushButton( "Cancel" );
	ok_canc_frame->layout()->addWidget( ok_canc_button );
	QObject::connect( ok_canc_button, &QPushButton::clicked, [frame]{
		frame->close();
	} );

	ok_canc_button = new QPushButton( "OK" );
	ok_canc_frame->layout()->addWidget( ok_canc_button );
	QObject::connect( ok_canc_button, &QPushButton::clicked, [this, pattern_combo, frame]{
		save_params( pattern_combo->currentText().toUtf8().constData() );
		frame->close();
	} );

	frame->layout()->addWidget(ok_canc_frame);

	return frame;
}

GstFlowReturn VideoTestSrc::new_appsink_sample( GstAppSink* appsink, gpointer object )
{
	GstSample* sample;
	VideoTestSrc* video_object = static_cast<VideoTestSrc*>( object );

	g_signal_emit_by_name( appsink, "pull-sample", &sample );

	GstCaps* caps = gst_sample_get_caps( sample );
	GstBuffer* buf = gst_sample_get_buffer( sample );

	GstStructure* caps_str = gst_caps_get_structure (caps, 0);
	int width, height;
	gst_structure_get_int( caps_str, "width", &width );
	gst_structure_get_int( caps_str, "height", &height );

	GstMapInfo map;
	gst_buffer_map( buf, &map, GST_MAP_READ);

	GstVideoInfo info;
	gst_video_info_from_caps( &info, caps );

	delete video_object->current_image;
	video_object->current_image = new Image( width, height, info.stride[0], 3, map.data, false );

	gst_buffer_unmap( buf, &map );

	if (sample)
		gst_sample_unref( sample );

	return GST_FLOW_OK;
}
