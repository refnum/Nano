/*	NAME:
		NTargetPOSIX.h

	DESCRIPTION:
		Thread-safe POSIX support.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_POSIX_HDR
#define NTARGET_POSIX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetPOSIX {
public:
	// math.h
	static bool							is_nan(float64_t x);
	static bool							is_inf(float64_t x);

	static float64_t					asin( float64_t x);
	static float64_t					acos( float64_t x);
	static float64_t					sinh( float64_t x);
	static float64_t					cosh( float64_t x);
	static float64_t					tanh( float64_t x);
	static float64_t					asinh(float64_t x);
	static float64_t					acosh(float64_t x);
	static float64_t					atanh(float64_t x);
	static float64_t					rint( float64_t x);


	// stdio.h
	static NIndex						snprintf(char *s, size_t n, const char *format, ...);


	// stdlib.h
	static int64_t						llabs(  int64_t  x);
	static void							srandom(uint32_t s);
	static uint32_t						random(void);


	// process.h
	static int							getpid(void);


	// unistd.h
	static char *						getcwd(char *buf, size_t size);
};





#endif // NTARGET_POSIX_HDR

