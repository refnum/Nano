/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFString.h"
#include "NMacTarget.h"
#include "NTargetThread.h"
#include "NTargetTime.h"





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTimeZone : Get a time zone.
//----------------------------------------------------------------------------
static NCFObject GetTimeZone(const NCFString &timeZone)
{	NCFObject		cfTimeZone;



	// Get the time zone
	if (timeZone == kNTimeZoneDefault)
		cfTimeZone.SetObject(CFTimeZoneCopyDefault());

	else
		cfTimeZone.SetObject(CFTimeZoneCreateWithName(kCFAllocatorNano, timeZone.GetObject(), true));

	NN_ASSERT(cfTimeZone.IsValid());

	return(cfTimeZone);
}





#pragma mark NTargetTime
//============================================================================
//		NTargetTime::ConvertTimeToDate : Convert a UTC time to a date.
//----------------------------------------------------------------------------
NGregorianDate NTargetTime::ConvertTimeToDate(NTime theTime, const NString &timeZone)
{	NCFObject			cfTimeZone;
	NGregorianDate		theDate;
	CFGregorianDate		cfDate;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	cfTimeZone = GetTimeZone(timeZone);
	cfTime     = theTime + kNEpochTimeSince2001;



	// Convert the time
	cfDate = CFAbsoluteTimeGetGregorianDate(cfTime, cfTimeZone);
	
	theDate.year     = cfDate.year;
	theDate.month    = cfDate.month;
	theDate.day      = cfDate.day;
	theDate.hour     = cfDate.hour;
	theDate.minute   = cfDate.minute;
	theDate.second   = cfDate.second;
	theDate.timeZone = NCFString(CFTimeZoneGetName(cfTimeZone), false);
	
	return(theDate);
}





//============================================================================
//		NTargetTime::ConvertDateToTime : Convert a date to a UTC time.
//----------------------------------------------------------------------------
NTime NTargetTime::ConvertDateToTime(const NGregorianDate &theDate)
{	NCFObject			cfTimeZone;
	NTime				theTime;
	CFGregorianDate		cfDate;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	cfTimeZone = GetTimeZone(theDate.timeZone);

	cfDate.year   = theDate.year;
	cfDate.month  = theDate.month;
	cfDate.day    = theDate.day;
	cfDate.hour   = theDate.hour;
	cfDate.minute = theDate.minute;
	cfDate.second = theDate.second;



	// Convert the date
	cfTime  = CFGregorianDateGetAbsoluteTime(cfDate, cfTimeZone);
	theTime = -kNEpochTimeSince2001 + cfTime;
	
	return(theTime);
}





//============================================================================
//		NTargetTime::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NTargetTime::GetDayOfWeek(const NGregorianDate &theDate)
{	NCFObject			cfTimeZone;
	NIndex				theIndex;
	NTime				theTime;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	theTime    = ConvertDateToTime(theDate);
	cfTimeZone = GetTimeZone(theDate.timeZone);
	cfTime     = theTime - kNEpochTimeSince2001;



	// Get the day of the week
	theIndex = CFAbsoluteTimeGetDayOfWeek(cfTime, cfTimeZone);
	NN_ASSERT(theIndex >= 1 && theIndex <= 7);
	
	return(theIndex);
}





