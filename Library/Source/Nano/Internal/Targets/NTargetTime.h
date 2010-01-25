/*	NAME:
		NTargetTime.h

	DESCRIPTION:
		Time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetTime {
public:
	// Get the current time
	static NTime						GetTime(void);


	// Get the time since boot
	static NTime						GetUpTime(void);


	// Manage timers
	//
	// Timers are always executed on the main thread.
	static NTimerID						TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery);
	static void							TimerDestroy(NTimerID theTimer);
	static void							TimerReset(  NTimerID theTimer, NTime fireAfter);
};





#endif // NTARGET_TIME_HDR

