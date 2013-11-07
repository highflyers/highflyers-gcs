#include "video_streamer/server/RtpServer.h"

using namespace HighFlyers;

//simple bus_call function from gst examples
gboolean RtpServer::bus_call(GstBus* bus, GstMessage* msg, gpointer data)
{
    GMainLoop* loop = (GMainLoop*)data;

    switch (GST_MESSAGE_TYPE(msg))
    {
    case GST_MESSAGE_EOS:
    {
        g_print ("End of stream\n");
        g_main_loop_quit(loop);
        break;
    }
    case GST_MESSAGE_ERROR:
    {
        gchar* debug;
        GError* err;

        gst_message_parse_error(msg, &err, &debug);
        g_printerr("Debugging info: %s\n", (debug) ? debug : "none");
        g_free(debug);

        g_print("Error: %s\n", err->message);
        g_error_free(err);

        g_main_loop_quit(loop);

        break;
    }
    default:
        break;
    }
    return TRUE;
}

RtpServer::RtpServer()
{
    //initialization
    src = NULL;
    _port = -1;
    _ip = NULL;

    gst_init(NULL, NULL);

    //create pipeline
    pipeline = gst_pipeline_new("pipeline");

    //create elements
    fmt = gst_element_factory_make("capsfilter", "fmt");
    video_rate = gst_element_factory_make("videorate", "video_rate");
    video_convert = gst_element_factory_make("videoconvert", "video_convert");
    payloader = gst_element_factory_make("rtpvrawpay", "payloader");
    sink = gst_element_factory_make("udpsink", "sink");

    if (!video_rate)
    {
        g_printerr("Failed to create video rate\n");
        return;
    }

    if (!video_convert)
    {
        g_printerr("Failed to create video convert\n");
        return;
    }

    if (!payloader)
    {
        g_printerr("Failed to create payloader\n");
        return;
    }

    if (!sink)
    {
        g_printerr("Failed to create sink\n");
        return;
    }

    g_object_set(sink, "sync", false, NULL);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, NULL);
    gst_object_unref(bus);
}

RtpServer::~RtpServer()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(sink));
    gst_object_unref(GST_OBJECT(payloader));
    gst_object_unref(GST_OBJECT(video_rate));
    gst_object_unref(GST_OBJECT(video_convert));
    gst_object_unref(GST_OBJECT(src));

    gst_object_unref(GST_OBJECT(pipeline));

    g_source_remove(bus_watch_id);
}

void RtpServer::set_port(int port)
{
    _port = port;
    g_object_set(sink, "port", port, NULL);
}

void RtpServer::set_host(const char* host)
{
    _ip = new char[strlen(host)+1];
    strcpy(_ip, host);

    g_object_set(sink, "host", host, NULL);
}

void RtpServer::set_video_source(const char* source, SourceType type)
{
    if (src != NULL) gst_object_unref(GST_OBJECT(src));

    GstCaps* fmtCaps;

    switch (type)
    {
    case V4lDevice :
        src = gst_element_factory_make("v4l2src", "source");
        g_object_set(src, "device", source, NULL);

        fmtCaps = gst_caps_from_string("video/x-raw, format=YUY2, width=640,"
                                       "height=480, framerate=30/1");

        break;
   /* case Uri :
        if (gst_uri_is_valid(source))
        {
            src = gst_element_factory_make("uridecodebin", "source");
            g_object_set(src, "uri", source, NULL);
        }
        else g_printerr("Invalid URI\n");
        break; */
    default:
        g_printerr("Invalid source type\n");
    }

    g_object_set(fmt, "caps", fmtCaps, NULL);
    gst_caps_unref(fmtCaps);

    if (!src)
    {
        g_printerr("Failed to create source\n");
        return;
    }
}

bool RtpServer::init_stream()
{
    if (!src || _port < 1 || !_ip) return false;

    //adding elements to pipeline
    gst_bin_add_many(GST_BIN(pipeline), src, fmt, video_rate, video_convert, payloader, sink, NULL);

    //linking
    if (!gst_element_link_many(src, fmt, video_rate, video_convert, payloader, sink, NULL))
    {
        g_warning ("Failed to link elements!");
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
