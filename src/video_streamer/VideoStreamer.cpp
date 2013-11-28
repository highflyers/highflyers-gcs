#include "video_streamer/VideoStreamer.h"
#include "video_streamer/DebugInfo.h"

using namespace HighFlyers;

gboolean VideoStreamer::bus_call( GstBus* bus, GstMessage* msg, gpointer data )
{
//	static_cast<VideoStreamer*>( data )->notify<GstMessage*>( &IVSObserver::bus_call, ( GstMessage* )msg );
	return TRUE;
}

#ifdef DEBUG
void VideoStreamer::debug_log_fnc( GstDebugCategory* category,
								   GstDebugLevel level,
								   const gchar* file,
								   const gchar* fnc,
								   gint line,
								   GObject* object,
								   GstDebugMessage* message,
								   gpointer data )
{
	DebugInfo info;
	info.category = category;
	info.file = file;
	info.fnc = fnc;
	info.line = line;
	info.object = object;
	info.message = message;
	static_cast<VideoStreamer*>( data )->notify<DebugInfo>( &IVSObserver::debug_log, info );
}
#endif

VideoStreamer::VideoStreamer()
{
	// initialization
	port = -1;
	ip = "";

	gst_init( NULL, NULL );

#ifdef DEBUG
	gst_debug_remove_log_function( gst_debug_log_default );
#endif

	// create pipeline
	pipeline = gst_pipeline_new( "pipeline" );
}

VideoStreamer::~VideoStreamer()
{
	gst_element_set_state( pipeline, GST_STATE_NULL );
	gst_object_unref( GST_OBJECT( pipeline ) );
	g_source_remove( bus_watch_id );
}

GstElement* VideoStreamer::create_gst_element_safe( const gchar* factory_name, const gchar* name )
{
	GstElement* result = gst_element_factory_make( factory_name, name );

	if ( !result )
	{
		std::string msg( "Failed to create element: " );
		msg += name;

		GST_ERROR( msg.c_str() );
		throw std::runtime_error( msg.c_str() );
	}

	return result;
}
