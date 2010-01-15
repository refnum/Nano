/*	NAME:
		NWinPOSIX.cpp

	DESCRIPTION:
		Windows POSIX support.
		
		Math functions derived from:
		
			http://msdn.microsoft.com/en-us/library/w3t84e33(VS.71).aspx
	
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
	return(::_isnan(r) != 0);
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
//      NTargetPOSIX::asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asin(Float64 x)
{


    // Get the value
	return(atan(x / sqrt(-x * x + 1.0)));
}





//============================================================================
//      NTargetPOSIX::acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acos(Float64 x)
{


    // Get the value
	return(atan(-x / sqrt(-x * x + 1.0)) + 2.0 * atan(1.0));
}





//============================================================================
//      NTargetPOSIX::sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::sinh(Float64 x)
{


    // Get the value
	return((exp(x) - exp(-x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::cosh(Float64 x)
{


    // Get the value
	return((exp(x) + exp(-x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::tanh(Float64 x)
{


    // Get the value
	return((exp(x) - exp(-x)) / (exp(x) + exp(-x)));
}





//============================================================================
//      NTargetPOSIX::asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asinh(Float64 x)
{


    // Get the value
	return(log(x + sqrt(x * x + 1.0)));
}





//============================================================================
//      NTargetPOSIX::acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acosh(Float64 x)
{


    // Get the value
	return(log(x + sqrt(x * x - 1.0)));
}





//============================================================================
//      NTargetPOSIX::atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::atanh(Float64 x)
{


    // Get the value
	return(log((1.0 + x) / (1.0 - x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::rint : Round to the nearest integer.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::rint(Float64 x)
{


    // Get the value
	return(floor(x + 0.5));
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


