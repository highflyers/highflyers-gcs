/*
 * FakeCommBuilder.cpp
 *
 *  Created on: 31 gru 2013
 *      Author: Marcin Kolny
 */

#include "FakeComm.h"

#ifdef _WIN32
#define EXPORT_DEF __declspec( dllexport )
#else
#define EXPORT_DEF
#endif

extern "C" EXPORT_DEF FakeComm* factory_method()
{
	return new FakeComm();
}

