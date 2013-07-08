/*	NAME:
		NLinuxPOSIX.cpp

	DESCRIPTION:
		Linux POSIX support.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetPOSIX.h"

#include <unistd.h>



//============================================================================
//		NTargetPOSIX::is_nan : Linux isnan.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_nan(Float64 x)
{


	// Get the value
	return(isnan(x));
}





//============================================================================
//		NTargetPOSIX::is_inf : Linux isinf.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_inf(Float64 x)
{


	// Get the value
	return(isinf(x));
}





//============================================================================
//      NTargetPOSIX::asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asin(Float64 x)
{


	// Get the value
	return(::asin(x));
}





//============================================================================
//      NTargetPOSIX::acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acos(Float64 x)
{


	// Get the value
	return(::acos(x));
}





//============================================================================
//      NTargetPOSIX::sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::sinh(Float64 x)
{


	// Get the value
	return(::sinh(x));
}





//============================================================================
//      NTargetPOSIX::cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::cosh(Float64 x)
{


	// Get the value
	return(::cosh(x));
}





//============================================================================
//      NTargetPOSIX::tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::tanh(Float64 x)
{


	// Get the value
	return(::tanh(x));
}





//============================================================================
//      NTargetPOSIX::asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asinh(Float64 x)
{


	// Get the value
	return(::asinh(x));
}





//============================================================================
//      NTargetPOSIX::acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acosh(Float64 x)
{


	// Get the value
	return(::acosh(x));
}





//============================================================================
//      NTargetPOSIX::atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::atanh(Float64 x)
{


	// Get the value
	return(::atanh(x));
}





//============================================================================
//      NTargetPOSIX::rint : Round to the nearest integer.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::rint(Float64 x)
{


	// Get the value
	return(::rint(x));
}





//============================================================================
//        NTargetPOSIX::snprintf : Linux snprintf.
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
	return(::llabs(x));
}





//============================================================================
//      NTargetPOSIX::srandom : Seed the random number generator.
//----------------------------------------------------------------------------
void NTargetPOSIX::srandom(UInt32 s)
{


	// Set the seed
	::srandom(s);
}





//============================================================================
//      NTargetPOSIX::random : Generate a random number.
//----------------------------------------------------------------------------
UInt32 NTargetPOSIX::random(void)
{


	// Get the value
	//
	// Random returns a long in 64-bit, so we need to mod the result to obtain
	// a 32-bit value that remains random.
	return((UInt32) (::random() % kUInt32Max));
}





//============================================================================
//      NTargetPOSIX::getpid : Get the process ID.
//----------------------------------------------------------------------------
int NTargetPOSIX::getpid(void)
{


	// Get the pid
	return(::getpid());
}





//============================================================================
//		NTargetPOSIX::getcwd : Linux getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(::getcwd(buf, size));
}


