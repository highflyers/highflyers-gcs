#ifndef _iobserver_h_
#define _iobserver_h_

namespace HighFlyers
{
class IVSObserver
{
public:
    typedef struct
    {
        GstDebugCategory *category;
        GstDebugLevel level;
        const gchar *file;
        const gchar *function;
        gint line;
        GObject *object;
        GstDebugMessage *message;
    } debugInfo;

private:
    GstMessage* last_bus_call_msg;
    debugInfo last_debug_msg;

public:
    virtual void debug_log(IObservable<IVSObserver>*, debugInfo msg)
    {
        last_debug_msg = msg;
    }

    debugInfo get_last_debug_log_msg()
    {
        return last_debug_msg;
    }


    virtual void bus_call(IObservable<IVSObserver>*, GstMessage* msg)
    {
        last_bus_call_msg = msg;
    }

    GstMessage* get_last_bus_call_msg()
    {
        return last_bus_call_msg;
    }
};
}

#endif
