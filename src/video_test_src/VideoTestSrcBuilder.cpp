/*
 * VideoTestSrcBuilder.cpp
 *
 *  Created on: 22 lis 2013
 *      Author: Marcin Kolny
 */

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

#include "video_test_src/VideoTestSrc.h"

extern "C" EXPORT_DEF HighFlyers::VideoTestSrc* factory_method()
{
	return new HighFlyers::VideoTestSrc();
}

