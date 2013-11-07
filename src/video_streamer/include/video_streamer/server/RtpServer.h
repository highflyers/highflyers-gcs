#ifndef _rtpserver_h_
#define _rtpserver_h_

#include <gst/gst.h>
#include <cstring>

/**
 * UDP video server class
 */
namespace HighFlyers
{
class RtpServer
{
private:

    /**< gstreamer pipeline */
    GstElement* pipeline;

    /**< source of video (v4l device or uri) */
    GstElement* src;

    /**< caps filter */
    GstElement* fmt;

    /**< video rate */
    GstElement* video_rate;

    /**< video convert */
    GstElement* video_convert;

    /**< rtp payloader */
    GstElement* payloader;

    /**< UDP output */
    GstElement* sink;

    /**< ID of GstBus */
    guint bus_watch_id;

    /**< UDP server port */
    int _port;

    /**< UDP server host name */
    char* _ip;

    /**< handler of forwarding messages from the streaming threads */
    static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data);

public:

    /**< Type of the video source */
    enum SourceType
    {
        //Uri, //TODO
        V4lDevice
    };

    RtpServer();
    ~RtpServer();

    /**
     * \param video source (e.g. /dev/video0 or file://fileName)
     * \param source type
     *
     * Set video to stream.
     */
    void set_video_source(const char* source, SourceType type);

    /**
     * \param hostname or IP Address
     *
     * host used by server
     */
    void set_host(const char* host);

    /**
    * \param port
    *
    * port used by server
    */
    void set_port(int port);

    /**
    * \return hostname or IP Address
    *
    * getter for host
    */
    char* get_host()
    {
        return _ip;
    }

    /**
    * \return port
    *
    * getter for port
    */
    int get_port()
    {
        return _port;
    }

    /**
    *
    * \return true if initialization success
    *
    * initialization of stream that you have to use
    * after setting all necessary server parameters.
    */
    bool init_stream();

    /**
    * Allows you to stop streaming
    */
    void stop_stream();

    /**
    * Allows you to start streaming
    */
    void start_stream();
};
}

#endif
