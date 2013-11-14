#include "video_streamer/server/RtpServer.h"

using namespace HighFlyers;

RtpServer::RtpServer()
{
    //initialization
    src = NULL;

    //create elements
    fmt = create_gst_element_safe("capsfilter", "fmt");
    video_rate = create_gst_element_safe("videorate", "video_rate");
    video_convert = create_gst_element_safe("videoconvert", "video_convert");
    payloader = create_gst_element_safe("rtpvrawpay", "payloader");
    sink = create_gst_element_safe("udpsink", "sink");

    g_object_set(sink, "sync", false, NULL);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, this);
    gst_object_unref(bus);

    // set debug log
    gst_debug_set_active(TRUE);
    gst_debug_set_default_threshold(GST_LEVEL_WARNING);
    gst_debug_add_log_function((GstLogFunction)debug_log_fnc, this, NULL);
}

RtpServer::~RtpServer()
{
}

void RtpServer::set_ip(const char* host)
{
    _ip = new char[strlen(host)+1];
    strcpy(_ip, host);

    g_object_set(sink, "host", host, NULL);
}


void RtpServer::set_port(int port)
{
    _port = port;
    g_object_set(sink, "port", port, NULL);
}

void RtpServer::set_video_source(const char* source, SourceType type)
{
    if (src != NULL) gst_object_unref(GST_OBJECT(src));

    GstCaps* fmtCaps;

    switch (type)
    {
    case V4lDevice :
        src = create_gst_element_safe("v4l2src", "source");
        g_object_set(src, "device", source, NULL);

        fmtCaps = gst_caps_from_string("video/x-raw, format=YUY2, width=640,"
                                       "height=480, framerate=30/1");

        break;
   /* case Uri :
        if (gst_uri_is_valid(source))
        {
            src = create_gst_element_safe("uridecodebin", "source");
            g_object_set(src, "uri", source, NULL);
        }
        else GST_ERROR("Invalid URI\n");
        break; */
    default:
        GST_ERROR("Invalid source type\n");
    }

    g_object_set(fmt, "caps", fmtCaps, NULL);
    gst_caps_unref(fmtCaps);
}

bool RtpServer::init_stream()
{
    if (!src || _port < 1 || !_ip) return false;

    //adding elements to pipeline
    gst_bin_add_many(GST_BIN(pipeline), src, fmt, video_rate, video_convert, payloader, sink, NULL);

    //linking
    if (!gst_element_link_many(src, fmt, video_rate, video_convert, payloader, sink, NULL))
    {
        throw std::runtime_error("Failed to link elements!");
        return false;
    }

    return true;
}

void RtpServer::start_stream()
{
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
}

void RtpServer::stop_stream()
{
}
