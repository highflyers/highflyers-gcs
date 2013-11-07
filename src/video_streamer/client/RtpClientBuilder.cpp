#include "video_streamer/client/RtpClient.h"

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

extern "C" EXPORT_DEF HighFlyers::RtpClient* factory_method()
{
	return new HighFlyers::RtpClient();
}