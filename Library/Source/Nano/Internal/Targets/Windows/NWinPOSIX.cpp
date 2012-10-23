/*	NAME:
		NWinPOSIX.cpp

	DESCRIPTION:
		Windows POSIX support.
		
		Math functions derived from:
		
			http://msdn.microsoft.com/en-us/library/w3t84e33(VS.71).aspx
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
//        NTargetPOSIX::snprintf : Windows snprintf.
//----------------------------------------------------------------------------
NIndex NTargetPOSIX::snprintf(char *s, size_t n, const char *format, ...)
{	NIndex		theResult;
	va_list		argList;



	// Print the value
	va_start(argList, format);
	theResult = vsnprintf(s, n, format, argList);
	va_end(argList);

	return(theResult);
}





//============================================================================
//      NTargetPOSIX::llabs : Absolute value of an integer.
//----------------------------------------------------------------------------
SInt64 NTargetPOSIX::llabs(SInt64 x)
{


    // Get the value
	return(x < 0 ? -x : x);
}





//============================================================================
//		NTargetPOSIX::getcwd : Windows getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(::_getcwd(buf, size));
}


