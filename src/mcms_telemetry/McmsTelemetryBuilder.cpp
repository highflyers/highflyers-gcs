/*
 * McmsTelemetryBuilder.cpp
 *
 *  Created on: 22 gru 2013
 *      Author: Marcin Kolny
 */

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

#include "mcms_telemetry/McmsTelemetry.h"

extern "C" EXPORT_DEF HighFlyers::McmsTelemetry* factory_method()
{
	return new HighFlyers::McmsTelemetry();
}



