#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <gst/gst.h>
#include <cstring>

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
     * \param IP Address
     *
     * IP Address
     */
    virtual void set_ip(const char* host) = 0;

    /**
    * \return hostname or IP Address
    *
    * getter for host
    */
    char* get_ip()
    {
        return _ip;
    }

    /**
    * \param port
    *
    * Port
    */
    virtual void set_port(int port) = 0;

    /**
    * \return port
    *
    * getter for port
    */
    int get_port()
    {
        return _port;
    }

protected:
    GstElement* create_gst_element_safe(const gchar *factoryname, const gchar *name);

    /**< handler of forwarding messages from the streaming threads */
    static gboolean bus_call(GstBus* bus, GstMessage* msg, gpointer data);

    static void debug_log_fnc(GstDebugCategory *category,
                              GstDebugLevel level,
                              const gchar *file,
                              const gchar *function,
                              gint line,
                              GObject *object,
                              GstDebugMessage *message,
                              gpointer user_data);

    /**< UDP server port */
    int _port;

    /**< UDP server host name */
    char* _ip;

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
