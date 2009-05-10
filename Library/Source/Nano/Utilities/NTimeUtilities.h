/*	NAME:
		NTimeUtilities.h

	DESCRIPTION:
		Time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMEUTILITIES_HDR
#define NTIMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimeUtilities {
public:
	// Get the current time
	static NTime						GetCurrentTime(void);


	// Get the time since boot
	static NTime						GetBootTime(void);
};





#endif // NTIMEUTILITIES_HDR


