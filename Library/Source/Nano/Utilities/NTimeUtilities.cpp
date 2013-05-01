/*	NAME:
		NTimeUtilities.cpp

	DESCRIPTION:
		Time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
//		NTimeUtilities::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetTime(void)
{


	// Get the time
	return(NTargetTime::GetTime());
}





//============================================================================
//		NTimeUtilities::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetUpTime(void)
{


	// Get the time
	return(NTargetTime::GetUpTime());
}

