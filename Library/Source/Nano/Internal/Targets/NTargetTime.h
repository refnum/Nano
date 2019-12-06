/*	NAME:
		NTargetTime.h

	DESCRIPTION:
		Time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_TIME_HDR
#define NTARGET_TIME_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimer.h"
#include "NDate.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetTime {
public:
	// Manage timers
	//
	// Timers are always executed on the main thread.
	static NTimerID						TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery);
	static void							TimerDestroy(NTimerID theTimer);
	static void							TimerReset(  NTimerID theTimer, NTime fireAfter);


	// Date/time conversion
	//
	// NTime values are always in UTC, relative to the Nano epoch, while gregorian
	// dates are always in a named time zone.
	//
	// Days of the week follow ISO-8601, in the range 1..7 for Mon..Sun.
	static NGregorianDate				ConvertTimeToDate(      NTime           theTime, const NString &timeZone);
	static NTime						ConvertDateToTime(const NGregorianDate &theDate);
	static NIndex						GetDayOfWeek(     const NGregorianDate &theDate);
};





#endif // NTARGET_TIME_HDR

