#include "video_streamer/server/RtpServer.h"

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

extern "C" EXPORT_DEF HighFlyers::RtpServer* factory_method()
{
	return new HighFlyers::RtpServer();
}
