/*	NAME:
		NTimeUtilities.h

	DESCRIPTION:
		Time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMEUTILITIES_HDR
#define NTIMEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDate.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimeUtilities {
public:
	// Get the current time
	//
	// Returns the time since the Nano Epoch (00:00:00 on 2001/01/01 in UTC).
	static NTime						GetTime(void);


	// Get the time since boot
	static NTime						GetUpTime(void);
};





#endif // NTIMEUTILITIES_HDR


