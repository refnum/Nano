/*	NAME:
		NTargetPOSIX.h

	DESCRIPTION:
		Thread-safe POSIX support.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
	static int							is_nan(double r);
	static int							is_inf(double r);

	
	// stdio.h
	static int							snprintf(char *s, size_t n, const char *format, ...);


	// time.h
	static struct tm					gmtime(time_t theTime);
	static time_t						timegm(const struct tm *tm);


	// unistd.h
	static char *						getcwd(char *buf, size_t size);
};





#endif // NTARGET_POSIX_HDR

