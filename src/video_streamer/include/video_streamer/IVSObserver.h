#ifndef IOBSERVER_H
#define IOBSERVER_H

namespace HighFlyers
{
class IVSObserver
{
public:
	typedef struct
	{
		GstDebugCategory* category;
		GstDebugLevel level;
		const gchar* file;
		const gchar* fnc;
		gint line;
		GObject* object;
		GstDebugMessage* message;
	} DebugInfo;

private:
	GstMessage* last_bus_call_msg;
	DebugInfo last_debug_msg;

public:
	virtual void debug_log( IObservable<IVSObserver>* obs, DebugInfo msg )
	{
		last_debug_msg = msg;
	}

	DebugInfo get_last_debug_log_msg()
	{
		return last_debug_msg;
	}


	virtual void bus_call( IObservable<IVSObserver>* obs, GstMessage* msg )
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
