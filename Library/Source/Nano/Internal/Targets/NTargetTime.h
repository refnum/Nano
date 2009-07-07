/*	NAME:
		NTargetTime.h

	DESCRIPTION:
		Time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_TIME_HDR
#define NTARGET_TIME_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetTime {
public:
	// Get the current time
	static NTime						GetTime(void);


	// Get the time since boot
	static NTime						GetUpTime(void);


	// Thread-safe gmtime
	static struct tm					gmtime(time_t theTime);
};





#endif // NTARGET_TIME_HDR

