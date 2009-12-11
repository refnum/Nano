/*	NAME:
		NWinTime.cpp

	DESCRIPTION:
		Windows time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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


// Timer state
typedef struct {
	NSpinLock			theLock;
	TimerInfoMap		theTimers;
} TimerState;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTimerState : Get the timer state.
//----------------------------------------------------------------------------
static TimerState *GetTimerState(void)
{	static TimerState	sTimerState;



	// Get the timer state
	return(&sTimerState);
}





//============================================================================
//		GetTimerInfo : Get a timer.
//----------------------------------------------------------------------------
static TimerInfo *GetTimerInfo(TimerState *timerState, UINT_PTR timerID)
{	TimerInfoMapIterator		theIter;



	// Get the timer
	theIter = timerState->theTimers.find(timerID);
	if (theIter != timerState->theTimers.end())
		return(theIter->second);

	return(NULL);
}





//============================================================================
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void CALLBACK TimerCallback(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR idEvent, DWORD /*dwTime*/)
{	TimerState		*timerState = GetTimerState();
	StLock			acquireLock(timerState->theLock);

	TimerInfo		*theInfo;
	UINT_PTR		timerID;



	// Get the state we need
	theInfo = GetTimerInfo(timerState, idEvent);
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
		timerID = SetTimer(NULL, theInfo->timerID, (UINT) (theInfo->fireEvery / kNTimeMillisecond), TimerCallback);
		NN_ASSERT(timerID == idEvent);
		}
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
{	TimerState		*timerState = GetTimerState();
	StLock			acquireLock(timerState->theLock);

	TimerInfo		*theInfo;



	// Validate our state
	NN_ASSERT(sizeof(UINT_PTR) <= sizeof(NTimerID));



	// Allocate the timer info
	theInfo = new TimerInfo;
	if (theInfo == NULL)
		return(kTimerNone);

	theInfo->timerID    = 0;
	theInfo->fireCount  = 0;
	theInfo->theFunctor = theFunctor;
	theInfo->fireAfter  = fireAfter;
	theInfo->fireEvery  = fireEvery;



	// Create the timer
	theInfo->timerID = SetTimer(NULL, 0, (UINT) (fireAfter / kNTimeMillisecond), TimerCallback);
	NN_ASSERT(theInfo->timerID != 0);
	
	if (theInfo->timerID == 0)
		{
		delete theInfo;
		return(kTimerNone);
		}



	// Save the timer
	timerState->theTimers[theInfo->timerID] = theInfo;
	
	return((NTimerID) theInfo);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{	TimerState		*timerState = GetTimerState();
	StLock			acquireLock(timerState->theLock);

	TimerInfo		*theInfo = (TimerInfo *) theTimer;
	BOOL			wasOK;
	


	// Remove the timer
	wasOK = KillTimer(NULL, theInfo->timerID);
	NN_ASSERT(wasOK);



	// Forget the timer
	timerState->theTimers.erase(theInfo->timerID);
	
	delete theInfo;
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{	TimerInfo		*theInfo = (TimerInfo *) theTimer;
	UINT_PTR		timerID;



	// Reset the timer
	theInfo->fireCount = 0;

	timerID = SetTimer(NULL, theInfo->timerID, (UINT) (fireAfter / kNTimeMillisecond), TimerCallback);
	NN_ASSERT(timerID == theInfo->timerID);
}




