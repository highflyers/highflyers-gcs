#include "video_streamer/client/RtpClient.h"

using namespace HighFlyers;

//simple bus_call function from gst examples
gboolean RtpClient::bus_call(GstBus* bus, GstMessage* msg, gpointer data)
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

RtpClient::RtpClient()
{
    // initialization
    _port = -1;
    _ip = NULL;

    gst_init(NULL, NULL);

    // create pipeline
    pipeline = gst_pipeline_new("pipeline");

    // create elements
    src = gst_element_factory_make("udpsrc", "source");
    depayloader = gst_element_factory_make("rtpvrawdepay", "depayloader");
    video_rate = gst_element_factory_make("videorate", "video_rate");
    fmt = gst_element_factory_make("capsfilter", "fmt");
    video_convert = gst_element_factory_make("videoconvert", "video_convert");
    queue1 = gst_element_factory_make("queue", "queue1");
    encoder = gst_element_factory_make("jpegenc", "encoder");
    queue2 = gst_element_factory_make("queue", "queue2");
    mux = gst_element_factory_make("avimux", "mux");
    sink = gst_element_factory_make("filesink", "sink");

    GstCaps* srcCaps = gst_caps_from_string("application/x-rtp, media=(string)video,"
                                            "sampling=(string)YCbCr-4:2:2, depth=(string)8,"
                                            "width=(string)640, height=(string)480");

    g_object_set(G_OBJECT(src), "caps", srcCaps, NULL);
    gst_caps_unref(srcCaps);

    // set framerate to 30fps
    GstCaps* filterCaps = gst_caps_new_simple ("video/x-raw",
                          "framerate", GST_TYPE_FRACTION, 30, 1,
                          NULL);

    g_object_set(G_OBJECT(fmt), "caps", filterCaps, NULL);
    gst_caps_unref(filterCaps);


    if (!src)
    {
        g_printerr("Failed to create encoder\n");
        return;
    }

    if (!depayloader)
    {
        g_printerr("Failed to create depayloader\n");
        return;
    }

    if (!video_rate)
    {
        g_printerr("Failed to create video rate\n");
        return;
    }

    if (!fmt)
    {
        g_printerr("Failed to create fmt\n");
        return;
    }

    if (!video_convert)
    {
        g_printerr("Failed to create video convert\n");
        return;
    }

    if (!queue1 || !queue2)
    {
        g_printerr("Failed to create queue\n");
        return;
    }

    if (!encoder)
    {
        g_printerr("Failed to create encoder\n");
        return;
    }

    if (!mux)
    {
        g_printerr("Failed to create mux\n");
        return;
    }

    if (!sink)
    {
        g_printerr("Failed to create sink\n");
        return;
    }

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
    bus_watch_id = gst_bus_add_watch(bus, bus_call, NULL);
    gst_object_unref(bus);

    // adding elements to pipeline
    gst_bin_add_many(GST_BIN(pipeline), src, depayloader, video_rate, fmt, video_convert,
                     queue1, encoder, queue2, mux, sink, NULL);

    // linking
    if (!gst_element_link_many(src, depayloader, video_rate, fmt, video_convert,
                               queue1, encoder, queue2, mux, sink, NULL))
    {
        g_warning ("Failed to link elements!");
    }
}

RtpClient::~RtpClient()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(sink));
    gst_object_unref(GST_OBJECT(mux));
    gst_object_unref(GST_OBJECT(queue2));
    gst_object_unref(GST_OBJECT(encoder));
    gst_object_unref(GST_OBJECT(queue1));
    gst_object_unref(GST_OBJECT(video_convert));
    gst_object_unref(GST_OBJECT(fmt));
    gst_object_unref(GST_OBJECT(video_rate));
    gst_object_unref(GST_OBJECT(depayloader));
    gst_object_unref(GST_OBJECT(src));

    gst_object_unref(GST_OBJECT(pipeline));

    g_source_remove(bus_watch_id);
}

void RtpClient::set_port(int port)
{
    _port = port;
    g_object_set(src, "port", port, NULL);
}

void RtpClient::set_ip(const char* host)
{
    _ip = new char[strlen(host)+1];
    strcpy(_ip, host);

    g_object_set(src, "address", host, NULL);
}

void RtpClient::read_to_file(const char* fileName)
{
    g_object_set(G_OBJECT(sink), "location", fileName, NULL);
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
}
