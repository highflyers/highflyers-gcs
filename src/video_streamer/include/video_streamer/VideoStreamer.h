#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <gst/gst.h>
#include <string>
#include <stdexcept>

#include "core/IObservable.h"
#include "video_streamer/IVSObserver.h"

namespace HighFlyers
{
class VideoStreamer : public IObservable<IVSObserver>
{
public:
	VideoStreamer();
	virtual ~VideoStreamer();

	/**
	 * \param host IP Address
	 *
	 * IP Address
	 */

	virtual void set_ip( const std::string& host ) = 0;

	/**
	* \return hostname or IP Address
	*
	* getter for host
	*/
	std::string get_ip()
	{
		if ( ip.empty() ) return "127.0.0.1";

		return ip;
	}

	/**
	* \param port Port
	*
	* Port
	*/
	virtual void set_port( int port ) = 0;

	/**
	* \return port
	*
	* getter for port
	*/
	int get_port()
	{
		return port;
	}

protected:
	GstElement* create_gst_element_safe( const gchar* factory_name, const gchar* name );

	/**< handler of forwarding messages from the streaming threads */
	static gboolean bus_call( GstBus* bus, GstMessage* msg, gpointer data );

#ifdef DEBUG
	static void debug_log_fnc( GstDebugCategory* category,
							   GstDebugLevel level,
							   const gchar* file,
							   const gchar* fnc,
							   gint line,
							   GObject* object,
							   GstDebugMessage* message,
							   gpointer user_data );
#endif

	/**< UDP server port */
	int port;

	/**< UDP server host name */
	std::string ip;

	/**< gstreamer pipeline */
	GstElement* pipeline;

	/**< source of video */
	GstElement* src;

	/**< output for video */
	GstElement* sink;

	/**< ID of GstBus */
	guint bus_watch_id;
};
}

#endif // VIDEOSTREAMER_H
