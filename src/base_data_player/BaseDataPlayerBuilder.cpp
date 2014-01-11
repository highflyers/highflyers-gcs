/*
 * BaseDataPlayerBuilder.cpp
 *
 *  Created on: 11 sty 2014
 *      Author: Marcin Kolny
 */

#include "BaseDataPlayer.h"

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

extern "C" EXPORT_DEF HighFlyers::BaseDataPlayer* factory_method()
{
	return new HighFlyers::BaseDataPlayer();
}

