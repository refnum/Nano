/*	NAME:
		NWinPOSIX.cpp

	DESCRIPTION:
		Windows POSIX support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <direct.h>

#include "NTargetPOSIX.h"





//============================================================================
//		NTargetPOSIX::is_nan : Windows isnan.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_nan(Float64 r)
{


	// Get the value
	return(::_isnan(r));
}





//============================================================================
//		NTargetPOSIX::is_inf : Windows isinf.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_inf(Float64 r)
{


	// Get the value
	return(!::_finite(r));
}





//============================================================================
//		NTargetPOSIX::snprintf : Windows snprintf.
//----------------------------------------------------------------------------
NIndex NTargetPOSIX::snprintf(char *s, size_t n, const char *format, ...)
{	NIndex		theResult;
	va_list		argList;



	// Print the value
	va_start(argList, format);
	theResult = ::vsnprintf(s, n, format, argList);
	va_end(argList);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::vsnprintf : Windows vsnprintf.
//----------------------------------------------------------------------------
NIndex NTargetPOSIX::vsnprintf(char *s, size_t n, const char *format, va_list argList)
{	NIndex		theResult;



	// Print the value
	theResult = ::vsnprintf(s, n, format, argList);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::gmtime : Windows gmtime.
//----------------------------------------------------------------------------
struct tm NTargetPOSIX::gmtime(time_t theTime)
{	struct tm		theResult;



	// Get the value
	//
	// gmtime uses thread-local storage on Windows, so is always thread-safe.
	theResult = *::gmtime(&theTime);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::timegm : Windows timegm.
//----------------------------------------------------------------------------
time_t NTargetPOSIX::timegm(const struct tm *tm)
{	struct tm	localTM = *tm;



	// Get the value
	return(::_mkgmtime32(&localTM));
}





//============================================================================
//		NTargetPOSIX::getcwd : Windows getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(::_getcwd(buf, size));
}


