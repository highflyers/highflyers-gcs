#ifndef _rtpclient_h_
#define _rtpclient_h_

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
};
}
#endif
