#include "video_streamer/client/RtpClient.h"

using namespace HighFlyers;

RtpClient::RtpClient()
{
	// create elements
	src = create_gst_element_safe( "udpsrc", "source" );
	depayloader = create_gst_element_safe( "rtpvrawdepay", "depayloader" );
	video_rate = create_gst_element_safe( "videorate", "video_rate" );
	fmt = create_gst_element_safe( "capsfilter", "fmt" );
	video_convert = create_gst_element_safe( "videoconvert", "video_convert" );
	queue1 = create_gst_element_safe( "queue", "queue1" );
	encoder = create_gst_element_safe( "jpegenc", "encoder" );
	queue2 = create_gst_element_safe( "queue", "queue2" );
	mux = create_gst_element_safe( "avimux", "mux" );
	sink = create_gst_element_safe( "filesink", "sink" );
	tee = create_gst_element_safe( "tee", "tee" );
	window_sink = create_gst_element_safe( "xvimagesink", "window_sink" );

	GstCaps* src_caps = gst_caps_from_string( "application/x-rtp, media=(string)video,"
						"sampling=(string)YCbCr-4:2:2, depth=(string)8,"
						"width=(string)640, height=(string)480" );

	g_object_set( G_OBJECT( src ), "caps", src_caps, NULL );
	gst_caps_unref( src_caps );

	// set framerate to 30fps
	GstCaps* filter_caps = gst_caps_new_simple( "video/x-raw", "framerate",
						   GST_TYPE_FRACTION, 30, 1, NULL );

	g_object_set( G_OBJECT( fmt ), "caps", filter_caps, NULL );
	gst_caps_unref( filter_caps );

	GstBus* bus = gst_pipeline_get_bus( GST_PIPELINE( pipeline ) );
	bus_watch_id = gst_bus_add_watch( bus, bus_call, this );
	gst_object_unref( bus );

	// set debug log
	gst_debug_set_active( FALSE );
	gst_debug_set_default_threshold( GST_LEVEL_WARNING );
	gst_debug_add_log_function( ( GstLogFunction )debug_log_fnc, this, NULL );

	// adding elements to pipeline
	gst_bin_add_many( GST_BIN( pipeline ), src, depayloader, video_rate, fmt, video_convert, tee, NULL );

	// linking
	if ( !gst_element_link_many( src, depayloader, video_rate, fmt, video_convert, tee, NULL ) )
	{
		throw std::runtime_error( "Failed to link elements!" );
	}
}

RtpClient::~RtpClient()
{
}

void RtpClient::set_ip( const char* host )
{
	ip = new char[strlen( host ) + 1];
	strcpy( ip, host );

	g_object_set( src, "address", host, NULL );
}

void RtpClient::set_port( int port )
{
	this->port = port;
	g_object_set( src, "port", port, NULL );
}

void RtpClient::read_to_file( const char* file_name )
{
	gst_bin_add_many( GST_BIN( pipeline ), queue1, encoder, queue2, mux, sink, NULL );

	if ( !gst_element_link_many( tee, queue1, encoder, queue2, mux, sink, NULL ) )
	{
		throw std::runtime_error( "Failed to link file sink!" );
	}

	g_object_set( G_OBJECT( sink ), "location", file_name, NULL );
}

void RtpClient::set_render_window( guintptr handler )
{
	gst_bin_add( GST_BIN( pipeline ), window_sink );

	if ( !gst_element_link_many( tee, window_sink, NULL ) )
	{
		throw std::runtime_error( "Failed to link window sink!" );
	}

	gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( window_sink ), handler );
}

void RtpClient::play()
{
	gst_element_set_state( GST_ELEMENT( pipeline ), GST_STATE_PLAYING );
}
