#ifndef _rtpclient_h_
#define _rtpclient_h_

#include <gst/video/videooverlay.h>

#include "video_streamer/VideoStreamer.h"

/**
 * UDP video client class
 */
namespace HighFlyers
{
class RtpClient : public VideoStreamer
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

public:
    RtpClient();
    ~RtpClient();

    /**
     * \param IP Address
     *
     * IP Address
     */
    void set_ip(const char* host);

    /**
    * \param port
    *
    * Port
    */
    void set_port(int port);

    /**
    * \param file name (mkv format)
    *
    * Save input stream to file
    */
    void read_to_file(const char* fileName);

    /**
    * \param window handler
    *
    * Render input stream on window
    */
    void set_render_window(guintptr handler);

    /**
    *
    * play stream
    */
    void play();
};
}
#endif
