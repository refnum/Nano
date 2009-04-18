/*	NAME:
		NWinTime.cpp

	DESCRIPTION:
		Windows time support.
	
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

	// dair, to do
	return(0.0);
}





//============================================================================
//		NTargetTime::GetBootTime : Get the time since booting.
//----------------------------------------------------------------------------
NTime NTargetTime::GetBootTime(void)
{


	// dair, to do; use QueryPerformanceTimer
	return(((NTime) GetTickCount()) * kNTimeMillisecond);
}

