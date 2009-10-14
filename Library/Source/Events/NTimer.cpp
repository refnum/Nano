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
//		Static variables
//----------------------------------------------------------------------------
EventLoopTimerUPP				NTimer::mTimerCallback = NewEventLoopTimerUPP(    TimerCallback);
EventLoopIdleTimerUPP			NTimer::mIdleCallback  = NewEventLoopIdleTimerUPP(IdleCallback);





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
{	bool		hasTimer;



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
{	EventLoopTimerRef	timerRef;
	OSStatus			theErr;



	// Install the timer
	timerRef = NULL;

	if (idleTimer)
		theErr = InstallEventLoopIdleTimer(eventLoop, timeDelay, timeRepeat, mIdleCallback,  this, &timerRef);
	else
		theErr = InstallEventLoopTimer(    eventLoop, timeDelay, timeRepeat, mTimerCallback, this, &timerRef);
	
	NN_ASSERT_NOERR(theErr);



	// Save the timer
	if (theErr == noErr)
		mTimers[timerRef] = theTimer;
	
	return(timerRef);
}





//============================================================================
//		NTimer::RemoveTimer : Remove a timer.
//----------------------------------------------------------------------------
void NTimer::RemoveTimer(NTimerID theTimer)
{	NTimerMapIterator		theIter;
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
{	NTimerMapIterator		theIter;
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
{	NTimerMapConstIterator		theIter;



	// Invoke the timer
	theIter = mTimers.find(theTimer);
	NN_ASSERT(theIter != mTimers.end());

	theIter->second(theMsg);
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

