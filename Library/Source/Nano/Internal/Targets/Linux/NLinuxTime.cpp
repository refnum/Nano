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

#include "NTargetTime.h"





//============================================================================
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
{	NGregorianDate		theDate = { 0 };



	// dair, to do
	NN_LOG("NTargetTime::ConvertTimeToDate not implemented!");
	return(theDate);
}





//============================================================================
//		NTargetTime::ConvertDateToTime : Convert a date to a UTC time.
//----------------------------------------------------------------------------
NTime NTargetTime::ConvertDateToTime(const NGregorianDate &theDate)
{


	// dair, to do
	NN_LOG("NTargetTime::ConvertDateToTime not implemented!");
	return(0.0);
}





//============================================================================
//		NTargetTime::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NTargetTime::GetDayOfWeek(const NGregorianDate &theDate)
{


	// dair, to do
	NN_LOG("NTargetTime::GetDayOfWeek not implemented!");
	return(0);
}





