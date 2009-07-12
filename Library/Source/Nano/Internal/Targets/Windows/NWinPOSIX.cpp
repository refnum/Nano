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
int NTargetPOSIX::is_nan(double r)
{


	// Get the value
	return(_isnan(r));
}





//============================================================================
//		NTargetPOSIX::is_inf : Windows isinf.
//----------------------------------------------------------------------------
int NTargetPOSIX::is_inf(double r)
{


	// Get the value
	return(!_finite(r));
}





//============================================================================
//		NTargetPOSIX::snprintf : Windows snprintf.
//----------------------------------------------------------------------------
int NTargetPOSIX::snprintf(char *s, size_t n, const char *format, ...)
{	int			theResult;
	va_list		argList;



	// Print the value
	va_start(argList, format);
	theResult = vsnprintf(s, n, format, argList);
	va_end(argList);
	
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
	return(_mkgmtime32(&localTM));
}





//============================================================================
//		NTargetPOSIX::getcwd : Windows getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(_getcwd(buf, size));
}


