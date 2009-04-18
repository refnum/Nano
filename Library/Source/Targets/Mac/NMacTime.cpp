/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetTime.h"





//============================================================================
//		NTargetTime::GetCurrentTime : Get the current time.
//----------------------------------------------------------------------------
NTime NTargetTime::GetCurrentTime(void)
{


	// Get the time
	return(CFAbsoluteTimeGetCurrent());
}





//============================================================================
//		NTargetTime::GetBootTime : Get the time since booting.
//----------------------------------------------------------------------------
NTime NTargetTime::GetBootTime(void)
{


	// Get the time
	return(GetCurrentEventTime());
}
