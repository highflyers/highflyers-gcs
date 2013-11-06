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

    //create main loop
    loop = g_main_loop_new(NULL, FALSE);

    //create pipeline
    pipeline = gst_pipeline_new("pipeline");

    //create elements
    encoder = gst_element_factory_make("x264enc", "encoder");
    payloader = gst_element_factory_make("rtph264pay", "payloader");
    sink = gst_element_factory_make("udpsink", "sink");

    if (!encoder)
    {
        g_printerr("Failed to create encoder\n");
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

    /* dynamicaly allocated payload type
    (see: http://en.wikipedia.org/wiki/RTP_audio_video_profile) */
    g_object_set(payloader, "pt", 96, NULL);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);
}

RtpServer::~RtpServer()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(sink));
    gst_object_unref(GST_OBJECT(payloader));
    gst_object_unref(GST_OBJECT(encoder));
    gst_object_unref(GST_OBJECT(src));

    gst_object_unref(GST_OBJECT(pipeline));

    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);
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

    switch (type)
    {
    case V4lDevice :
        src = gst_element_factory_make("v4l2src", "source");
        g_object_set(src, "device", source, NULL);
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
    gst_bin_add_many(GST_BIN(pipeline), src, encoder, payloader, sink, NULL);

    //linking
    if (!gst_element_link_many(src, encoder, payloader, sink, NULL))
    {
        g_warning ("Failed to link elements!");
        return false;
    }

    return true;
}

void RtpServer::start_stream()
{
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);

    g_main_loop_run(loop);
}

void RtpServer::stop_stream()
{
}
