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
//		NTimeUtilities::GetCurrentTime : Get the current time.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetCurrentTime(void)
{


	// Get the time
	return(NTargetTime::GetCurrentTime());
}





//============================================================================
//		NTimeUtilities::GetBootTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetBootTime(void)
{


	// Get the time
	return(NTargetTime::GetBootTime());
}





//============================================================================
//		NTimeUtilities::GetBootTimeMS : Get the time since boot in milliseconds.
//----------------------------------------------------------------------------
UInt32 NTimeUtilities::GetBootTimeMS(void)
{


	// Get the time
	return((UInt32) (NTargetTime::GetBootTime() / kNTimeMillisecond));
}





//============================================================================
//		NTimeUtilities::GetBootTimeUS : Get the time since boot in microseconds.
//----------------------------------------------------------------------------
UInt64 NTimeUtilities::GetBootTimeUS(void)
{


	// Get the time
	return((UInt64) (NTargetTime::GetBootTime() / kNTimeMicrosecond));
}




