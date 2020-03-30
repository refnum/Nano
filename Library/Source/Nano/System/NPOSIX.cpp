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
#include <stdlib.h>

#if NN_TARGET_WINDOWS
	#include <io.h>
#else
	#include <unistd.h>
#endif





//=============================================================================
//		NPOSIX::getenv : Get an environment variable.
//-----------------------------------------------------------------------------
NString NPOSIX::getenv(const NString& theName)
{


	// Get the value
	NString theValue;

#if NN_TARGET_WINDOWS
	const wchar_t* winName  = reinterpret_cast<const wchar_t*>(theName.GetUTF16());
	const wchar_t* winValue = _wgetenv(winName);
	if (winValue != nullptr)
	{
		const utf16_t* envValue = reinterpret_cast<const utf16_t*>(winValue);
		theValue                = NString(envValue);
	}
#else
	const utf8_t* envValue = ::getenv(theName.GetUTF8());
	if (envValue != nullptr)
	{
		theValue = envValue;
	}
#endif

	return theValue;
}





//=============================================================================
//		NPOSIX::setenv : Set an environment variable.
//-----------------------------------------------------------------------------
void NPOSIX::setenv(const NString& theName, const NString& theValue)
{
#if NN_TARGET_WINDOWS
	NString nameValue = theName + "=" + theValue;

	int sysErr = _wputenv(reinterpret_cast<const wchar_t*>(nameValue.GetUTF16()));
	NN_EXPECT_NOT_ERR(sysErr);

#else
	int sysErr = ::setenv(theName.GetUTF8(), theValue.GetUTF8(), 1);
	NN_EXPECT_NOT_ERR(sysErr);
#endif
}





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
