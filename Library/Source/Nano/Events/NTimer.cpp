/*	NAME:
		NTimer.cpp

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetTime.h"
#include "NTimer.h"





//============================================================================
//		NTimer::NTimer : Constructor.
//----------------------------------------------------------------------------
NTimer::NTimer(void)
{
}





//============================================================================
//		NTimer::~NTimer : Destructor.
//----------------------------------------------------------------------------
NTimer::~NTimer(void)
{


	// Clean up
	RemoveTimer();
}





//============================================================================
//		NTimer::HasTimer : Do we have a timer?
//----------------------------------------------------------------------------
bool NTimer::HasTimer(NTimerID theTimer) const
{	StLock		acquireLock(mLock);
	bool		hasTimer;



	// Check our state
	if (theTimer == kNTimerAny)
		hasTimer = !mTimers.empty();
	else
		hasTimer = (mTimers.find(theTimer) != mTimers.end());
	
	return(hasTimer);
}





//============================================================================
//		NTimer::AddTimer : Add a timer.
//----------------------------------------------------------------------------
NTimerID NTimer::AddTimer(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery)
{	StLock		acquireLock(mLock);
	NTimerID	theID;



	// Create th etimer
	theID = NTargetTime::TimerCreate(theFunctor, fireAfter, fireEvery);
	if (theID != kNTimerNone)
		{
		NN_ASSERT(mTimers.find(theID) == mTimers.end());
		mTimers[theID] = theFunctor;
		}
	
	return(theID);
}





//============================================================================
//		NTimer::RemoveTimer : Remove a timer.
//----------------------------------------------------------------------------
void NTimer::RemoveTimer(NTimerID theTimer)
{	StLock					acquireLock(mLock);
	NTimerMapIterator		theIter;



	// Remove all timers
	if (theTimer == kNTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			NTargetTime::TimerDestroy(theIter->first);
		
		mTimers.clear();
		}
	
	
	
	// Remove a single timer
	else
		{
		theIter = mTimers.find(theTimer);
		NN_ASSERT(theIter != mTimers.end());

		if (theIter != mTimers.end())
			{
			NTargetTime::TimerDestroy(theIter->first);
			mTimers.erase(theIter);
			}
		}
}





//============================================================================
//		NTimer::ResetTimer : Reset a timer.
//----------------------------------------------------------------------------
void NTimer::ResetTimer(NTime fireAfter, NTimerID theTimer)
{	StLock					acquireLock(mLock);
	NTimerMapIterator		theIter;



	// Adjust all timers
	if (theTimer == kNTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			NTargetTime::TimerReset(theIter->first, fireAfter);
		}
	
	
	
	// Adjust a single timer
	else
		{
		theIter = mTimers.find(theTimer);
		NN_ASSERT(theIter != mTimers.end());
		
		if (theIter != mTimers.end())
			NTargetTime::TimerReset(theIter->first, fireAfter);
		}
}



