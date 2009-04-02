/*	NAME:
		NTimeUtilities.cpp

	DESCRIPTION:
		Time utilities.
	
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
#include "NTimeUtilities.h"





//============================================================================
//		NTimeUtilities::GetTimeInMS : Get the time in milliseconds.
//----------------------------------------------------------------------------
UInt32 NTimeUtilities::GetTimeInMS(void)
{


	// Get the time
	return((UInt32) (NTargetTime::GetTime() / kNTimeMillisecond));
}





//============================================================================
//		NTimeUtilities::GetTimeInUS : Get the time in microseconds.
//----------------------------------------------------------------------------
UInt64 NTimeUtilities::GetTimeInUS(void)
{


	// Get the time
	return((UInt64) (NTargetTime::GetTime() / kNTimeMicrosecond));
}





//============================================================================
//		NTimeUtilities::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetTime(void)
{


	// Get the time
	return(NTargetTime::GetTime());
}


