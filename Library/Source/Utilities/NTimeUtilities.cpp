/*	NAME:
		NTimeUtilities.cpp

	DESCRIPTION:
		Time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"





//============================================================================
//		NTimeUtilities::GetTimeInMS : Get the time in milliseconds.
//----------------------------------------------------------------------------
UInt32 NTimeUtilities::GetTimeInMS(void)
{


	// Get the time
	return((UInt32) (NTimeUtilities::GetTimeInUS() / 1000));
}





//============================================================================
//		NTimeUtilities::GetTimeInUS : Get the time in microseconds.
//----------------------------------------------------------------------------
UInt64 NTimeUtilities::GetTimeInUS(void)
{	Nanoseconds		nanoSecs;
	UInt64			theTime;



	// Get the time
	nanoSecs = AbsoluteToNanoseconds(UpTime());
	theTime  = UnsignedWideToUInt64(nanoSecs) / 1000;
	
	return(theTime);
}





//============================================================================
//		NTimeUtilities::GetDateInterval : Get the interval between two dates.
//----------------------------------------------------------------------------
CFGregorianUnits NTimeUtilities::GetDateInterval(const NDate &timeStart, const NDate &timeEnd, CFGregorianUnitFlags theUnits)
{	CFGregorianUnits	theInterval;



	// Get the interval
	theInterval = CFAbsoluteTimeGetDifferenceAsGregorianUnits(timeEnd.GetTime(), timeStart.GetTime(), NULL, theUnits);
	
	return(theInterval);
}





//============================================================================
//		NTimeUtilities::GetGregorianDate : Convert a LongDateRec to a CFGregorianDate.
//----------------------------------------------------------------------------
CFGregorianDate NTimeUtilities::GetGregorianDate(const LongDateRec &longDate)
{	CFGregorianDate		gregDate;



	// Validate our parameters
	NN_ASSERT(longDate.ld.month  >= 1 && longDate.ld.month  <= 12);
	NN_ASSERT(longDate.ld.day    >= 1 && longDate.ld.day    <= 31);
	NN_ASSERT(longDate.ld.hour   >= 0 && longDate.ld.hour   <= 23);
	NN_ASSERT(longDate.ld.minute >= 0 && longDate.ld.minute <= 59);
	NN_ASSERT(longDate.ld.second >= 0 && longDate.ld.second <= 59);
	


	// Convert the date
	gregDate.year   = (SInt32) longDate.ld.year;
	gregDate.month  = (SInt8 ) longDate.ld.month;
	gregDate.day    = (SInt8 ) longDate.ld.day;
	gregDate.hour   = (SInt8 ) longDate.ld.hour;
	gregDate.minute = (SInt8 ) longDate.ld.minute;
	gregDate.second = (double) longDate.ld.second;
	
	return(gregDate);
}





//============================================================================
//		NTimeUtilities::GetLongDate : Convert a CFGregorianDate to a LongDateRec.
//----------------------------------------------------------------------------
LongDateRec NTimeUtilities::GetLongDate(const CFGregorianDate &gregDate)
{	LongDateRec		longDate;



	// Validate our parameters
	NN_ASSERT(gregDate.month  >= 1   && gregDate.month  <= 12);
	NN_ASSERT(gregDate.day    >= 1   && gregDate.day    <= 31);
	NN_ASSERT(gregDate.hour   >= 0   && gregDate.hour   <= 23);
	NN_ASSERT(gregDate.minute >= 0   && gregDate.minute <= 59);
	NN_ASSERT(gregDate.second >= 0.0 && gregDate.second <= 59.0);
	


	// Convert the date
	longDate.ld.year   = (SInt16) gregDate.year;
	longDate.ld.month  = (SInt16) gregDate.month;
	longDate.ld.day    = (SInt16) gregDate.day;
	longDate.ld.hour   = (SInt16) gregDate.hour;
	longDate.ld.minute = (SInt16) gregDate.minute;
	longDate.ld.second = (SInt16) gregDate.second;
	
	return(longDate);
}





//============================================================================
//		NTimeUtilities::GetGregorianDate : Convert a LongDateTime to a CFGregorianDate.
//----------------------------------------------------------------------------
CFGregorianDate NTimeUtilities::GetGregorianDate(const LongDateTime &longTime)
{	LongDateRec		longDate;



	// Convert the date
	LongSecondsToDate(&longTime, &longDate);
	
	return(NTimeUtilities::GetGregorianDate(longDate));
}





//============================================================================
//		NTimeUtilities::GetLongTime : Convert a CFGregorianDate to a LongDateTime.
//----------------------------------------------------------------------------
LongDateTime NTimeUtilities::GetLongTime(const CFGregorianDate &gregDate)
{	LongDateTime	longTime;
	LongDateRec		longDate;



	// Convert the date
	longDate = NTimeUtilities::GetLongDate(gregDate);
	
	LongDateToSeconds(&longDate, &longTime);
	
	return(longTime);
}





