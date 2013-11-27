#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "DebugInfo.h"

namespace HighFlyers
{
class IVSObserver
{
private:
	GstMessage* last_bus_call_msg;
#ifdef DEBUG
	DebugInfo last_debug_msg;
#endif

public:
#ifdef DEBUG
	virtual void debug_log( IObservable<IVSObserver>* sender, DebugInfo msg )
	{
		last_debug_msg = msg;
	}

	DebugInfo get_last_debug_log_msg()
	{
		return last_debug_msg;
	}
#endif

	virtual void bus_call( IObservable<IVSObserver>* sender, GstMessage* msg )
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
