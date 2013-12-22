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
class RtpClient : public IVideoSourcePlugin, public VideoStreamer
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

	/**< UDP caps */
	std::string udp_caps;

	/**
	 * video caps
	 */
	std::string video_caps;

	unsigned int window_handler;

	Image* current_image;

	/**
	 * Gets udp caps
	 */
	std::string get_udp_caps() const;

	/**
	 * Gets video caps
	 */
	std::string get_video_caps() const;

	/**
	 * \param caps udp caps
	 *
	 * Sets udp caps
	 */
	void set_udp_caps( const std::string& caps );

	/**
	 * \param caps video caps
	 *
	 * Sets video caps
	 */
	void set_video_caps( const std::string& caps );

public:
	RtpClient();
	~RtpClient();

	/**
	 * \param host IP Address
	 *
	 * IP Address
	 */
	void set_ip( const std::string& host );

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
