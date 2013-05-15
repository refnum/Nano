/*	NAME:
		NLinuxTime.cpp

	DESCRIPTION:
		Linux time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/time.h>

#include "NLock.h"
#include "NTargetTime.h"





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static NSpinLock gTimeZoneLock;
static char		*gTimeZoneOld;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		LockTimeZone : Lock the time zone.
//----------------------------------------------------------------------------
static void LockTimeZone(const NString &timeZone)
{


	// Lock the time zone
	//
	// Manipulating the environment is the only way to select the time zone used
	// by POSIX time conversion functions.
	//
	// We use a lock to ensure this is 'thread safe', in the sense that Nano will
	// not clash with itself (unfortunately 3rd party threads may still change this
	// from under us).
	gTimeZoneLock.Lock();
	gTimeZoneOld = getenv("TZ");

	if (timeZone == kNTimeZoneDefault)
		unsetenv("TZ");
	else
		setenv("TZ", timeZone.GetUTF8(), 1);

	tzset();



	// Check for failure
	//
	// Errors can only be detected by checking that a request for a non-UTC timezone
	// did in fact select a non-UTC timezone.
	if (timeZone != kNTimeZoneUTC)
		{
		NN_ASSERT(strcmp(tzname[0], "UTC") != 0);
		NN_ASSERT(strcmp(tzname[1], "UTC") != 0);
		}
}





//============================================================================
//		UnlockTimeZone : Unlock the time zone.
//----------------------------------------------------------------------------
static void UnlockTimeZone(void)
{


	// Unlock the time zone
	if (gTimeZoneOld != NULL)
		setenv("TZ", gTimeZoneOld, 1);
	else
		unsetenv("TZ");
	
	tzset();

	gTimeZoneOld = NULL;
	gTimeZoneLock.Unlock();
}





//============================================================================
//		Public functions
//----------------------------------------------------------------------------
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
#pragma mark -
NTime NTargetTime::GetTime(void)
{	struct timeval		timeVal;
	NTime				theTime;



	// Get the time
	gettimeofday(&timeVal, NULL);
	theTime = ((NTime) timeVal.tv_sec) + (((NTime) timeVal.tv_usec) * kNTimeMicrosecond);
	
	return(theTime);
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{	struct timespec		timeSpec;
	NTime				theTime;
	int					sysErr;



	// Get the state we need
	sysErr = clock_gettime(CLOCK_MONOTONIC, &timeSpec);
	NN_ASSERT_NOERR(sysErr);
	
	
	
	// Get the time since boot
	theTime = 0.0;
	
	if (sysErr == 0)
		theTime = ((NTime) timeSpec.tv_sec) + (((NTime) timeSpec.tv_nsec) * kNTimeNanosecond);

	return(theTime);
}





//============================================================================
//		NTargetTime::TimerCreate : Create a timer.
//----------------------------------------------------------------------------
NTimerID NTargetTime::TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery)
{


	// dair, to do
	NN_LOG("NTargetTime::TimerCreate not implemented!");
	return(0);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{


	// dair, to do
	NN_LOG("NTargetTime::TimerDestroy not implemented!");
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{


	// dair, to do
	NN_LOG("NTargetTime::TimerReset not implemented!");
}





//============================================================================
//		NTargetTime::ConvertTimeToDate : Convert a UTC time to a date.
//----------------------------------------------------------------------------
NGregorianDate NTargetTime::ConvertTimeToDate(NTime theTime, const NString &timeZone)
{	struct tm			zoneTime, queryTime;
	NTime				intSec, fracSec;
	time_t				sysTime;
	NGregorianDate		theDate;



	// Get the state we need
	memset(&zoneTime,  0x00, sizeof(zoneTime));
	memset(&queryTime, 0x00, sizeof(queryTime));

	intSec  = floor(theTime);
	fracSec = theTime - intSec;



	// Convert the time
	LockTimeZone(timeZone);

		// Get the offset
		//
		// Converting the UTC time to the equivalent date in the target time zone gives us,
		// via a Linux/BSD extension to struct tm, the offset of that time at that date in
		// the target time zone.
		sysTime = (time_t) (intSec + kNEpochTimeSince1970);
		localtime_r(&sysTime, &zoneTime);

		queryTime.tm_year  = zoneTime.tm_year;
		queryTime.tm_mon   = zoneTime.tm_mon;
		queryTime.tm_mday  = zoneTime.tm_mday;
		queryTime.tm_hour  = zoneTime.tm_hour;
		queryTime.tm_min   = zoneTime.tm_min;
		queryTime.tm_sec   = zoneTime.tm_sec;
		queryTime.tm_isdst = -1;
		
		mktime(&queryTime);


		// Convert the time
		//
		// We can then adjust the UTC time by that offset to obtain the equivalent date to
		// that UTC time in the target time zone.
		sysTime += queryTime.tm_gmtoff;
		gmtime_r(&sysTime, &zoneTime);

		theDate.year     = zoneTime.tm_year + 1900;
		theDate.month    = zoneTime.tm_mon  + 1;
		theDate.day      = zoneTime.tm_mday;
		theDate.hour     = zoneTime.tm_hour;
		theDate.minute   = zoneTime.tm_min;
		theDate.second   = ((NTime) zoneTime.tm_sec) + fracSec;
		theDate.timeZone = timeZone;

	UnlockTimeZone();

	return(theDate);
}





//============================================================================
//		NTargetTime::ConvertDateToTime : Convert a date to a UTC time.
//----------------------------------------------------------------------------
NTime NTargetTime::ConvertDateToTime(const NGregorianDate &theDate)
{	NTime			intSec, fracSec;
	struct tm		zoneTime;
	time_t			sysTime;
	NTime			theTime;



	// Get the state we need
	memset(&zoneTime, 0x00, sizeof(zoneTime));

	intSec  = floor(theDate.second);
	fracSec = theDate.second - intSec;



	// Convert the date
	//
	// mktime gives us the UTC time for the specified date in the target time zone,
	// which we can then adjust by the required epoch to obtain an NTime.
	LockTimeZone(theDate.timeZone);

		zoneTime.tm_year  = theDate.year - 1900;
		zoneTime.tm_mon   = theDate.month;
		zoneTime.tm_mday  = theDate.day;
		zoneTime.tm_hour  = theDate.hour;
		zoneTime.tm_min   = theDate.minute;
		zoneTime.tm_sec   = (int) intSec;
		zoneTime.tm_isdst = -1;

		sysTime = mktime(&zoneTime);
		theTime = -kNEpochTimeSince1970 + sysTime + fracSec;

	UnlockTimeZone();

	return(theTime);
}





//============================================================================
//		NTargetTime::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NTargetTime::GetDayOfWeek(const NGregorianDate &theDate)
{	struct tm		zoneTime;
	NIndex			theIndex;



	// Get the state we need
	memset(&zoneTime, 0x00, sizeof(zoneTime));



	// Get the day of the week
	//
	// mktime gives us the UTC time for the specified date in the target time zone.
	LockTimeZone(theDate.timeZone);

		zoneTime.tm_year  = theDate.year - 1900;
		zoneTime.tm_mon   = theDate.month;
		zoneTime.tm_mday  = theDate.day;
		zoneTime.tm_hour  = theDate.hour;
		zoneTime.tm_min   = theDate.minute;
		zoneTime.tm_sec   = (int) theDate.second;
		zoneTime.tm_isdst = -1;

		mktime(&zoneTime);

		theIndex = zoneTime.tm_wday + 1;
		NN_ASSERT(theIndex >= 1 && theIndex <= 7);

	UnlockTimeZone();

	return(theIndex);
}





