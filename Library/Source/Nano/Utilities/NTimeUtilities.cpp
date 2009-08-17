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





//============================================================================
//		NTimeUtilities::GetInterval : Get the interval between two dates.
//----------------------------------------------------------------------------
NGregorianUnits NTimeUtilities::GetInterval(const NDate &theStart, const NDate &theEnd)
{


	// Get the interval
	return(GetInterval(theStart.GetTime(), theEnd.GetTime()));
}





//============================================================================
//		NTimeUtilities::GetInterval : Get the interval between two times.
//----------------------------------------------------------------------------
NGregorianUnits NTimeUtilities::GetInterval(const NTime &theStart, const NTime &theEnd)
{	NGregorianUnits		theResult;
	NTime				theDelta;



	// Calculate the interval
	//
	// This is an approximation for now, and is inaccurate for durations
	// longer than a month (with the longer the duration, the worse it is).
	//
	// Should be implemented by taking the start time then adding intervals
	// until the end time is reached.
	memset(&theResult, 0x00, sizeof(theResult));
	theDelta = theEnd - theStart;
	
	while (theDelta > 0.0)
		{
		if (theDelta >= kNTimeYearish)
			{
			theResult.years += 1;
			theDelta        -= kNTimeYearish;
			}
		
		else if (theDelta >= kNTimeMonthish)
			{
			theResult.months += 1;
			theDelta         -= kNTimeMonthish;
			}

		else if (theDelta >= kNTimeDay)
			{
			theResult.days += 1;
			theDelta       -= kNTimeDay;
			}
		
		else if (theDelta >= kNTimeHour)
			{
			theResult.hours += 1;
			theDelta        -= kNTimeHour;
			}
		
		else if (theDelta >= kNTimeMinute)
			{
			theResult.minutes += 1;
			theDelta          -= kNTimeMinute;
			}

		else if (theDelta > 0.0)
			{
			theResult.seconds = theDelta;
			break;
			}
		}

	return(theResult);
}




