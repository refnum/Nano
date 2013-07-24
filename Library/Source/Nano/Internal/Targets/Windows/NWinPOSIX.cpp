/*	NAME:
		NWinPOSIX.cpp

	DESCRIPTION:
		Windows POSIX support.
		
		Math functions derived from:
		
			http://msdn.microsoft.com/en-us/library/w3t84e33(VS.71).aspx
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <direct.h>
#include <process.h>

#include "NTargetPOSIX.h"





//============================================================================
//		NTargetPOSIX::is_nan : Windows isnan.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_nan(float64_t r)
{


	// Get the value
	return(::_isnan(r) != 0);
}





//============================================================================
//		NTargetPOSIX::is_inf : Windows isinf.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_inf(float64_t r)
{


	// Get the value
	return(!::_finite(r));
}





//============================================================================
//      NTargetPOSIX::asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::asin(float64_t x)
{


    // Get the value
	return(atan(x / sqrt(-x * x + 1.0)));
}





//============================================================================
//      NTargetPOSIX::acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::acos(float64_t x)
{


    // Get the value
	return(atan(-x / sqrt(-x * x + 1.0)) + 2.0 * atan(1.0));
}





//============================================================================
//      NTargetPOSIX::sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::sinh(float64_t x)
{


    // Get the value
	return((exp(x) - exp(-x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::cosh(float64_t x)
{


    // Get the value
	return((exp(x) + exp(-x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::tanh(float64_t x)
{


    // Get the value
	return((exp(x) - exp(-x)) / (exp(x) + exp(-x)));
}





//============================================================================
//      NTargetPOSIX::asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::asinh(float64_t x)
{


    // Get the value
	return(log(x + sqrt(x * x + 1.0)));
}





//============================================================================
//      NTargetPOSIX::acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::acosh(float64_t x)
{


    // Get the value
	return(log(x + sqrt(x * x - 1.0)));
}





//============================================================================
//      NTargetPOSIX::atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::atanh(float64_t x)
{


    // Get the value
	return(log((1.0 + x) / (1.0 - x)) / 2.0);
}





//============================================================================
//      NTargetPOSIX::rint : Round to the nearest integer.
//----------------------------------------------------------------------------
float64_t NTargetPOSIX::rint(float64_t x)
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
int64_t NTargetPOSIX::llabs(int64_t x)
{


    // Get the value
	return(x < 0 ? -x : x);
}





//============================================================================
//      NTargetPOSIX::srandom : Seed the random number generator.
//----------------------------------------------------------------------------
void NTargetPOSIX::srandom(uint32_t s)
{


	// Set the seed
	::srand(s);
}





//============================================================================
//      NTargetPOSIX::random : Generate a random number.
//----------------------------------------------------------------------------
uint32_t NTargetPOSIX::random(void)
{


	// Get the value
	return(::rand());
}





//============================================================================
//      NTargetPOSIX::getpid : Get the process ID.
//----------------------------------------------------------------------------
int NTargetPOSIX::getpid(void)
{


    // Get the pid
	return(::_getpid());
}





//============================================================================
//		NTargetPOSIX::getcwd : Windows getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(::_getcwd(buf, size));
}


