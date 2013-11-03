#include "include/RtpClient.h"

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

    // create main loop
    loop = g_main_loop_new(NULL, FALSE);

    // create pipeline
    pipeline = gst_pipeline_new("pipeline");

    // create elements
    src = gst_element_factory_make("udpsrc", "source");
    buffer = gst_element_factory_make("rtpjitterbuffer", "buffer");
    depayloader = gst_element_factory_make("rtph264depay", "depayloader");
    parser = gst_element_factory_make("h264parse", "parser");
    filter = gst_element_factory_make("capsfilter", "filter");
    mux = gst_element_factory_make("matroskamux", "mux");
    sink = gst_element_factory_make("filesink", "sink");

    GstCaps* srcCaps = gst_caps_new_simple ("application/x-rtp",
                                            "payload", G_TYPE_INT, 96,
                                            NULL);

    g_object_set(G_OBJECT(src), "caps", srcCaps, NULL);
    gst_caps_unref(srcCaps);

    // set framerate to 30fps
    GstCaps* filterCaps = gst_caps_new_simple ("video/x-h264",
                          "framerate", GST_TYPE_FRACTION, 30, 1,
                          NULL);

    g_object_set(G_OBJECT(filter), "caps", filterCaps, NULL);
    gst_caps_unref(filterCaps);


    if (!src)
    {
        g_printerr("Failed to create encoder\n");
        return;
    }

    if (!buffer)
    {
        g_printerr("Failed to create buffer\n");
        return;
    }

    if (!depayloader)
    {
        g_printerr("Failed to create depayloader\n");
        return;
    }

    if (!parser)
    {
        g_printerr("Failed to create parser\n");
        return;
    }

    if (!filter)
    {
        g_printerr("Failed to create filter\n");
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
    bus_watch_id = gst_bus_add_watch(bus, bus_call, loop);
    gst_object_unref(bus);

    // adding elements to pipeline
    gst_bin_add_many(GST_BIN(pipeline), src, buffer, depayloader, parser, filter, mux, sink, NULL);

    // linking
    if (!gst_element_link_many(src, buffer, depayloader, parser, filter, mux, sink, NULL))
    {
        g_warning ("Failed to link elements!");
    }
}

RtpClient::~RtpClient()
{
    gst_element_set_state (pipeline, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(sink));
    gst_object_unref(GST_OBJECT(mux));
    gst_object_unref(GST_OBJECT(filter));
    gst_object_unref(GST_OBJECT(parser));
    gst_object_unref(GST_OBJECT(depayloader));
    gst_object_unref(GST_OBJECT(buffer));
    gst_object_unref(GST_OBJECT(src));

    gst_object_unref(GST_OBJECT(pipeline));

    g_source_remove(bus_watch_id);
    g_main_loop_unref(loop);
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

    g_main_loop_run(loop);
}
