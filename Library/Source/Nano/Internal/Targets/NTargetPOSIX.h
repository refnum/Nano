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
	static bool							is_nan(Float64 x);
	static bool							is_inf(Float64 x);

	static Float64						asin( Float64 x);
	static Float64						acos( Float64 x);
	static Float64						sinh( Float64 x);
	static Float64						cosh( Float64 x);
	static Float64						tanh( Float64 x);
	static Float64						asinh(Float64 x);
	static Float64						acosh(Float64 x);
	static Float64						atanh(Float64 x);
	static Float64						rint( Float64 x);


	// stdio.h
	static NIndex						snprintf( char *s, size_t n, const char *format, ...);


	// stdlib.h
	static SInt64						llabs(  SInt64 x);
	static void							srandom(UInt32 s);
	static UInt32						random(void);


	// process.h
	static int							getpid(void);


	// unistd.h
	static char *						getcwd(char *buf, size_t size);
};





#endif // NTARGET_POSIX_HDR

