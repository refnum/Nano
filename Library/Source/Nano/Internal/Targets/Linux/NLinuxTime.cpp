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

#include "NSpinLock.h"
#include "NTargetTime.h"





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static NSpinLock gTimeZoneLock;
static char		*gTimeZoneOld;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTZName : Get a time zone name.
//----------------------------------------------------------------------------
static const char *GetTZName(const NString &timeZone)
{	const char		*tzName;



	// Get the name
	//
	// The tz database has its own set of  time zone names:
	//
	//		http://en.wikipedia.org/wiki/List_of_tz_database_time_zones
	//
		 if (timeZone == kNTimeZoneUTC)		tzName = "UTC";

	else if (timeZone == kNTimeZoneWET)		tzName = "WET";
	else if (timeZone == kNTimeZoneCET)		tzName = "CET";
	else if (timeZone == kNTimeZoneEET)		tzName = "EET";
	else if (timeZone == kNTimeZoneCEST)	tzName = "CET";
	else if (timeZone == kNTimeZoneBST)		tzName = "Europe/London";
	else if (timeZone == kNTimeZoneIST)		tzName = "Asia/Calcutta";

	else if (timeZone == kNTimeZonePST)		tzName = "PST8PDT";
	else if (timeZone == kNTimeZoneMST)		tzName = "MST";
	else if (timeZone == kNTimeZoneCST)		tzName = "CST6CDT";
	else if (timeZone == kNTimeZoneEST)		tzName = "EST";
	else if (timeZone == kNTimeZonePDT)		tzName = "PST8PDT";
	else if (timeZone == kNTimeZoneMDT)		tzName = "MST7MDT";
	else if (timeZone == kNTimeZoneCDT)		tzName = "CST6CDT";
	else if (timeZone == kNTimeZoneEDT)		tzName = "EST5EDT";

	else
		{
		NN_LOG("Unknown time zone: [%@]", timeZone);
		tzName = NULL;
		}

	return(tzName);
}





//============================================================================
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
		setenv("TZ", GetTZName(timeZone), 1);

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





#pragma mark NTargetTime
//============================================================================
//		Public functions
//----------------------------------------------------------------------------
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
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
{	NTime				intSec, fracSec;
	struct tm			zoneTime;
	time_t				sysTime;
	NGregorianDate		theDate;



	// Get the state we need
	memset(&zoneTime, 0x00, sizeof(zoneTime));

	intSec  = floor(theTime);
	fracSec = theTime - intSec;



	// Convert the time
	LockTimeZone(timeZone);

		sysTime = (time_t) (intSec + kNEpochTimeSince1970);
		localtime_r(&sysTime, &zoneTime);

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
	LockTimeZone(theDate.timeZone);

		zoneTime.tm_year  = theDate.year  - 1900;
		zoneTime.tm_mon   = theDate.month - 1;
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
	LockTimeZone(theDate.timeZone);

		zoneTime.tm_year  = theDate.year  - 1900;
		zoneTime.tm_mon   = theDate.month - 1;
		zoneTime.tm_mday  = theDate.day;
		zoneTime.tm_hour  = theDate.hour;
		zoneTime.tm_min   = theDate.minute;
		zoneTime.tm_sec   = (int) theDate.second;
		zoneTime.tm_isdst = -1;

		(void) mktime(&zoneTime);

		theIndex = (zoneTime.tm_wday == 0) ? 7 : zoneTime.tm_wday;
		NN_ASSERT(theIndex >= 1 && theIndex <= 7);

	UnlockTimeZone();

	return(theIndex);
}





