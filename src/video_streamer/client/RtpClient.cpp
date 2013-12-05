#include "video_streamer/client/RtpClient.h"

using namespace HighFlyers;

RtpClient::RtpClient()
	: window_handler( 0 ),
	  current_image( nullptr )
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
#ifdef DEBUG
	gst_debug_set_active( TRUE );
	gst_debug_set_default_threshold( GST_LEVEL_WARNING );
	gst_debug_add_log_function( ( GstLogFunction )debug_log_fnc, this, NULL );
#endif

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
	delete current_image;
}

void RtpClient::set_ip( const std::string& host )
{
	ip = host.c_str();

	if ( ip.empty() ) ip = "127.0.0.1";

	g_object_set( src, "address", ip.c_str(), NULL );
}

void RtpClient::set_port( int port )
{
	this->port = port;
	g_object_set( src, "port", port, NULL );
}

void RtpClient::set_filename( const std::string& filename )
{
	g_object_set( G_OBJECT( sink ), "location", filename.c_str(), NULL );
}

void RtpClient::set_render_window( unsigned int handler )
{
	window_handler = handler;
	gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( window_sink ), handler );
}

void RtpClient::play( bool recording )
{
	//TODO Dynamically changing the pipeline

	if ( recording )
	{
		gst_bin_add_many( GST_BIN( pipeline ), queue1, encoder, queue2, mux, sink, NULL );

		if ( !gst_element_link_many( tee, queue1, encoder, queue2, mux, sink, NULL ) )
		{
			throw std::runtime_error( "Failed to link file sink!" );
		}
	}

	gst_bin_add( GST_BIN( pipeline ), window_sink );

	if ( !gst_element_link( tee, window_sink ) )
	{
		throw std::runtime_error( "Failed to link window sink!" );
	}

	if ( window_handler != 0 )
		set_render_window( window_handler );

	gst_element_set_state( GST_ELEMENT( pipeline ), GST_STATE_PLAYING );
}

void RtpClient::stop()
{
	gst_element_set_state( GST_ELEMENT( pipeline ), GST_STATE_NULL );
}

Image* RtpClient::get_image()
{
	return current_image;
}

QWidget* RtpClient::get_config_window()
{
	auto frame = new QFrame();

	frame->setLayout( new QVBoxLayout() );

	auto settingBox = new QGroupBox();
	auto le_ip = new QLineEdit( QString::fromStdString( get_ip() ) );
	auto sb_port = new QSpinBox();
	sb_port->setRange( 0, 65535 );
	sb_port->setValue( get_port() );


	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow( "IP: ", le_ip );
	formLayout->addRow( "Port: ", sb_port );
	settingBox->setLayout( formLayout );

	frame->layout()->addWidget( settingBox );
	settingBox->setTitle( "Connection" );

	auto ok_canc_frame = new QFrame();
	ok_canc_frame->setLayout( new QHBoxLayout() );

	auto ok_canc_button = new QPushButton( "Cancel" );
	ok_canc_frame->layout()->addWidget( ok_canc_button );
	QObject::connect( ok_canc_button, &QPushButton::clicked, [frame]
	{
		frame->close();
	} );

	ok_canc_button = new QPushButton( "OK" );
	ok_canc_frame->layout()->addWidget( ok_canc_button );
	QObject::connect( ok_canc_button, &QPushButton::clicked, [this, le_ip, sb_port, frame]
	{
		set_ip( le_ip->text().toStdString() );
		set_port( sb_port->value() );
		frame->close();
	} );

	frame->layout()->addWidget( ok_canc_frame );

	return frame;
}
