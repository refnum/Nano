/*	NAME:
		NTimer.cpp

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
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
{	StLock		lockMutex(mLock);
	bool		hasTimer;



	// Check our state
	if (theTimer == kTimerAny)
		hasTimer = !mTimers.empty();
	else
		hasTimer = (mTimers.find(theTimer) != mTimers.end());
	
	return(hasTimer);
}





//============================================================================
//		NTimer::AddTimer : Add a timer.
//----------------------------------------------------------------------------
NTimerID NTimer::AddTimer(	NTimerFunctor	theTimer,
							EventTime		timeDelay,
							EventTime		timeRepeat,
							bool			idleTimer,
							EventLoopRef	eventLoop)
{	StLock					lockMutex(mLock);
	EventLoopTimerRef		timerRef;
	OSStatus				theErr;



	// Install the timer
	timerRef = NULL;

	if (idleTimer)
		theErr = InstallEventLoopIdleTimer(eventLoop, timeDelay, timeRepeat, IdleCallback,  this, &timerRef);
	else
		theErr = InstallEventLoopTimer(    eventLoop, timeDelay, timeRepeat, TimerCallback, this, &timerRef);
	
	NN_ASSERT_NOERR(theErr);



	// Save the reference
	if (theErr == noErr)
		{
		NN_ASSERT(mTimers.find(timerRef) == mTimers.end());
		mTimers[timerRef] = theTimer;
		}
	
	return(timerRef);
}





//============================================================================
//		NTimer::RemoveTimer : Remove a timer.
//----------------------------------------------------------------------------
void NTimer::RemoveTimer(NTimerID theTimer)
{	StLock					lockMutex(mLock);
	NTimerMapIterator		theIter;
	OSStatus				theErr;



	// Remove all timers
	if (theTimer == kTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			{
			theErr = RemoveEventLoopTimer(theIter->first);
			NN_ASSERT_NOERR(theErr);
			}
		
		mTimers.clear();
		}
	
	
	
	// Remove a single timer
	else
		{
		theIter = mTimers.find(theTimer);
		if (theIter != mTimers.end())
			{
			theErr = RemoveEventLoopTimer(theIter->first);
			NN_ASSERT_NOERR(theErr);
			
			mTimers.erase(theIter);
			}
		}
}





//============================================================================
//		NTimer::SetNextFireTime : Set the next fire time.
//----------------------------------------------------------------------------
void NTimer::SetNextFireTime(EventTime theTime, NTimerID theTimer)
{	StLock					lockMutex(mLock);
	NTimerMapIterator		theIter;
	OSStatus				theErr;



	// Adjust all timers
	if (theTimer == kTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			{
			theErr = SetEventLoopTimerNextFireTime(theIter->first, theTime);
			NN_ASSERT_NOERR(theErr);
			}
		}
	
	
	
	// Adjust a single timer
	else
		{
		theIter = mTimers.find(theTimer);
		if (theIter != mTimers.end())
			{
			theErr = SetEventLoopTimerNextFireTime(theIter->first, theTime);
			NN_ASSERT_NOERR(theErr);
			}
		}
}





//============================================================================
//		NTimer::InvokeTimer : Invoke a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NTimer::InvokeTimer(EventLoopTimerRef theTimer, TimerMsg theMsg)
{	NTimerFunctor				theFunctor;
	NTimerMapConstIterator		theIter;



	// Get the functor
	//
	// We must acquire the lock to obtain the functor, since another
	// thread might be manipulating us while we are executing.
	mLock.Lock();

	theIter    = mTimers.find(theTimer);
	theFunctor = theIter->second;

	mLock.Unlock();



	// Invoke the functor
	//
	// The functor must be invoked after the lock is released, since it may be
	// used to destroy this object (and so we can't touch any of our state after
	// it returns).
	NN_ASSERT(theFunctor != NULL);
	theFunctor(theMsg);
}





//============================================================================
//		NTimer::TimerCallback : Timer callback.
//----------------------------------------------------------------------------
void NTimer::TimerCallback(EventLoopTimerRef theTimer, void *userData)
{	NTimer	*thisPtr = (NTimer *) userData;



	// Invoke the timer
	thisPtr->InvokeTimer(theTimer, kMsgTimerFire);
}





//============================================================================
//		NTimer::IdleCallback : Idle timer callback.
//----------------------------------------------------------------------------
void NTimer::IdleCallback(EventLoopTimerRef theTimer, EventLoopIdleTimerMessage theMsg, void *userData)
{	NTimer	*thisPtr = (NTimer *) userData;



	// Invoke the timer
	thisPtr->InvokeTimer(theTimer, (TimerMsg) theMsg);
}

