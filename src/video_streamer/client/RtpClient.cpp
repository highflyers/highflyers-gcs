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
	gst_bin_add_many( GST_BIN( pipeline ), src, depayloader, video_rate, fmt, video_convert, tee, window_sink, NULL );

	// linking
	if ( !gst_element_link_many( src, depayloader, video_rate, fmt, video_convert, tee, window_sink, NULL ) )
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

	g_object_set( src, "multicast-group", ip.c_str(), NULL );
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

std::string RtpClient::get_udp_caps() const
{
	if ( udp_caps.empty() ) return "application/x-rtp, media=(string)video,"
			"clock-rate=90000, encoding-name=RAW, sampling=(string)YCbCr-4:2:0,"
			"depth=(string)8, width=(string)640, height=(string)480";

	return udp_caps;
}

std::string RtpClient::get_video_caps() const
{
	if ( video_caps.empty() ) return "video/x-raw,width=640,height=480,framerate=24/1";

	return udp_caps;
}

void RtpClient::set_udp_caps( const std::string& caps )
{
	udp_caps = caps;
	GstCaps* src_caps = gst_caps_from_string( caps.c_str() );

	g_object_set( G_OBJECT( src ), "caps", src_caps, NULL );
	gst_caps_unref( src_caps );
}

void RtpClient::set_video_caps( const std::string& caps )
{
	video_caps = caps;
	GstCaps* filter_caps = gst_caps_from_string( caps.c_str() );

	g_object_set( G_OBJECT( fmt ), "caps", filter_caps, NULL );
	gst_caps_unref( filter_caps );
}

QWidget* RtpClient::get_config_window()
{
	auto frame = new QFrame();

	frame->setLayout( new QVBoxLayout() );

	auto settigs_box = new QGroupBox();
	auto le_ip = new QLineEdit( QString::fromStdString( get_ip() ) );
	auto sb_port = new QSpinBox();
	auto le_udp_caps = new QLineEdit( QString::fromStdString( get_udp_caps() ) );
	auto le_video_caps = new QLineEdit( QString::fromStdString( get_video_caps() ) );
	sb_port->setRange( 0, 65535 );
	sb_port->setValue( get_port() );


	QFormLayout* formLayout = new QFormLayout;
	formLayout->addRow( "IP: ", le_ip );
	formLayout->addRow( "Port: ", sb_port );
	formLayout->addRow( "UDP caps: ", le_udp_caps );
	formLayout->addRow( "Video caps: ", le_video_caps );
	settigs_box->setLayout( formLayout );

	frame->layout()->addWidget( settigs_box );
	settigs_box->setTitle( "Connection" );

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
	QObject::connect( ok_canc_button, &QPushButton::clicked, [this, le_ip, sb_port, le_udp_caps, le_video_caps, frame]
	{
		set_ip( le_ip->text().toStdString() );
		set_port( sb_port->value() );
		set_udp_caps( le_udp_caps->text().toStdString() );
		set_video_caps( le_video_caps->text().toStdString() );
		frame->close();
	} );

	frame->layout()->addWidget( ok_canc_frame );

	return frame;
}
