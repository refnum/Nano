/*	NAME:
		NPOSIX.cpp

	DESCRIPTION:
		POSIX glue.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
		All rights reserved.

		This software is distributed under the terms of your licensing
		agreement with refNum Software.
		
		This license grants you permission to use, copy, modify, or
		distribute this sofware only under the terms of that license.
		
		refNum Software retains full ownership of this software.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPOSIX.h"


// Nano
#include "NanoTargets.h"


// System
#if NN_TARGET_WINDOWS
	#include <io.h>
#else
	#include <unistd.h>
#endif





//=============================================================================
//		NPOSIX::localtime : Convert a time_t to a struct tm.
//-----------------------------------------------------------------------------
struct tm NPOSIX::localtime(time_t timeUnix)
{
	// Convert the time
	//
	// Windows has _s, rather than _r, and reverses the parameter order.
	struct tm localTime
	{
	};

#if NN_TARGET_WINDOWS
	localtime_s(&localTime, &timeUnix);
#else
	localtime_r(&timeUnix, &localTime);
#endif

	return localTime;
}
