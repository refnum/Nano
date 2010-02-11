/*	NAME:
		NWinTime.cpp

	DESCRIPTION:
		Windows time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <mmsystem.h>

#include "NDate.h"
#include "NWindows.h"
#include "NTargetThread.h"
#include "NTargetTime.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Timer info
typedef struct {
	UINT				timerID;
	NIndex				fireCount;
	NTimerFunctor		theFunctor;
	NTime				fireAfter;
	NTime				fireEvery;
} TimerInfo;


// Maps
typedef std::map<UINT_PTR, TimerInfo*>						TimerInfoMap;
typedef TimerInfoMap::iterator								TimerInfoMapIterator;
typedef TimerInfoMapIterator::const_iterator				TimerInfoMapIteratorConstIterator;





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static TimerInfoMap gTimers;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTimerMS : Get a timer time.
//----------------------------------------------------------------------------
static DWORD GetTimerMS(NTime theTime)
{	DWORD		timeMS;



	// Get the time
	timeMS = NWinTarget::ConvertTimeMS(theTime);
	timeMS = NN_CLAMP_VALUE(theTime, (UINT) USER_TIMER_MINIMUM, (UINT) USER_TIMER_MAXIMUM);
	
	return(timeMS);
}





//============================================================================
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void CALLBACK TimerCallback(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR idEvent, DWORD /*dwTime*/)
{	TimerInfoMapIterator	theIter;
	TimerInfo				*theInfo;
	UINT_PTR				timerID;



	// Get the state we need
	theIter = gTimers.find(timerID);
	NN_ASSERT(theIter != gTimers.end());

	theInfo = theIter->second;
	NN_ASSERT(theInfo != NULL);



	// Invoke the timer
	theInfo->fireCount++;
	theInfo->theFunctor(kNTimerFired);



	// Reschedule the timer
	//
	// Since Windows timers only have a single fire time, we may
	// need to reschedule the timer after it has fired once.
	if (theInfo->fireCount == 1 && !NMathUtilities::IsZero(theInfo->fireEvery))
		{
		timerID = SetTimer(NULL, theInfo->timerID, GetTimerMS(theInfo->fireEvery), TimerCallback);
		NN_ASSERT(timerID == idEvent);
		}
}





//============================================================================
//		DoTimerCreate : Create a timer.
//----------------------------------------------------------------------------
static void DoTimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery, NTimerID *theID)
{	TimerInfo		*theInfo;



	// Validate our state
	NN_ASSERT(sizeof(UINT_PTR) <= sizeof(NTimerID));



	// Allocate the timer info
	*theID  = kTimerNone;
	theInfo = new TimerInfo;

	if (theInfo == NULL)
		return;

	theInfo->timerID    = 0;
	theInfo->fireCount  = 0;
	theInfo->theFunctor = theFunctor;
	theInfo->fireAfter  = fireAfter;
	theInfo->fireEvery  = fireEvery;



	// Create the timer
	theInfo->timerID = SetTimer(NULL, 0, GetTimerMS(fireAfter), TimerCallback);
	NN_ASSERT(theInfo->timerID != 0);
	
	if (theInfo->timerID == 0)
		{
		delete theInfo;
		return;
		}



	// Save the timer
	gTimers[theInfo->timerID] = theInfo;
	*theID = (NTimerID) theInfo;
}





//============================================================================
//		DoTimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
static void DoTimerDestroy(NTimerID theTimer)
{	TimerInfo		*theInfo = (TimerInfo *) theTimer;
	BOOL			wasOK;
	


	// Remove the timer
	wasOK = KillTimer(NULL, theInfo->timerID);
	NN_ASSERT(wasOK);



	// Forget the timer
	gTimers.erase(theInfo->timerID);
	
	delete theInfo;
}





//============================================================================
//		DoTimerReset : Reset a timer.
//----------------------------------------------------------------------------
static void DoTimerReset(NTimerID theTimer, NTime fireAfter)
{	TimerInfo		*theInfo = (TimerInfo *) theTimer;
	UINT_PTR		timerID;
	DWORD			timeMS;



	// Reset the timer
	theInfo->fireCount = 0;

	timerID = SetTimer(NULL, theInfo->timerID, GetTimerMS(fireAfter), TimerCallback);
	NN_ASSERT(timerID == theInfo->timerID);
}





//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
#pragma mark -
NTime NTargetTime::GetTime(void)
{	UInt64			hectoNanoSecs;
	FILETIME		winTime;
	NTime			theTime;



	// Get the time
	GetSystemTimeAsFileTime(&winTime);

	hectoNanoSecs = ToNN(winTime.dwHighDateTime, winTime.dwLowDateTime);
	theTime       = ((NTime) hectoNanoSecs / 10) * kNTimeMicrosecond;
	theTime      -= kNEpochTimeSince1601;
	
	return(theTime);
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{


	// Get the time
	return(((NTime) GetTickCount()) * kNTimeMillisecond);
}





//============================================================================
//		NTargetTime::TimerCreate : Create a timer.
//----------------------------------------------------------------------------
NTimerID NTargetTime::TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery)
{	NTimerID		theID;



	// Create the timer
	//
	// Timers are per-thread on Windows, however Nano always executes functors on
	// the main thread and so we need to create the functor on the main thread.
	theID = kNTimerNone;
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerCreate(theFunctor, fireAfter, fireEvery, &theID);

	return(theID);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{


	// Destroy the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerDestroy(theTimer));
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{


	// Reset the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerReset(theTimer, fireAfter));
}




