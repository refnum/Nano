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
#include "NTargetTime.h"





//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
#pragma mark -
NTime NTargetTime::GetTime(void)
{


	// dair, to do
	NN_LOG("NTargetTime::GetTime not implemented!");
	return(0.0);
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{


	// dair, to do
	NN_LOG("NTargetTime::GetUpTime not implemented!");
	return(0.0);
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





