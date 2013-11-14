#include "video_streamer/VideoStreamer.h"

using namespace HighFlyers;

gboolean VideoStreamer::bus_call(GstBus* bus, GstMessage* msg, gpointer data)
{
    ((VideoStreamer*)data)->notify<GstMessage*>(&IVSObserver::bus_call, (GstMessage*)msg);
    return TRUE;
}

void VideoStreamer::debug_log_fnc(GstDebugCategory *category,
                                  GstDebugLevel level,
                                  const gchar *file,
                                  const gchar *function,
                                  gint line,
                                  GObject *object,
                                  GstDebugMessage *message,
                                  gpointer data)
{
    IVSObserver::debugInfo info;
    info.category = category;
    info.file = file;
    info.function = function;
    info.line = line;
    info.object = object;
    info.message = message;
    ((VideoStreamer*)data)->notify<IVSObserver::debugInfo>(&IVSObserver::debug_log, info);
}

VideoStreamer::VideoStreamer()
{
    // initialization
    _port = -1;
    _ip = NULL;

    gst_init(NULL, NULL);

    gst_debug_remove_log_function(gst_debug_log_default);

    // create pipeline
    pipeline = gst_pipeline_new("pipeline");
}

VideoStreamer::~VideoStreamer()
{
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_source_remove(bus_watch_id);
}

GstElement* VideoStreamer::create_gst_element_safe(const gchar *factoryname, const gchar *name)
{
    GstElement* result = gst_element_factory_make(factoryname, name);
    if (!result)
    {
        std::string msg("Failed to create element: ");
        msg += name;

        GST_ERROR(msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
    return result;
}
