#ifndef RTPCLIENT_H
#define RTPCLIENT_H

#include <gst/video/videooverlay.h>
#include <QtWidgets>

#include "core/plugin_interfaces/IVideoSourcePlugin.h"
#include "core/Image.h"

#include "video_streamer/VideoStreamer.h"

/**
 * UDP video client class
 */
namespace HighFlyers
{
class RtpClient : public VideoStreamer, IVideoSourcePlugin
{
private:
	/**< rtp depayloader */
	GstElement* depayloader;

	/**< video_rate */
	GstElement* video_rate;

	/**< fmt */
	GstElement* fmt;

	/**< video convert */
	GstElement* video_convert;

	/**< queue */
	GstElement* queue1;

	/**< encoder */
	GstElement* encoder;

	/**< queue */
	GstElement* queue2;

	/**< muxer */
	GstElement* mux;

	/**< tee */
	GstElement* tee;

	/**< window sink */
	GstElement* window_sink;

	unsigned int window_handler;

	Image* current_image;

public:
	RtpClient();
	~RtpClient();

	/**
	 * \param host IP Address
	 *
	 * IP Address
	 */
	void set_ip( std::string host );

	/**
	* \param port Port
	*
	* Port
	*/
	void set_port( int port );

	/**
	* \param file_name file name (mkv format)
	*
	* Save input stream to file
	*/
	void set_filename( const std::string& filename );

	/**
	* \param handler window handler
	*
	* Render input stream on window
	*/
	void set_render_window( unsigned int handler );

	/**
	*
	* play stream
	*/
	void play( bool recording );

	/**
	*
	* stop stream
	*/
	void stop();

	Image* get_image();

	QWidget* get_config_window();
};
}
#endif
