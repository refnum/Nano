/*	NAME:
		NTimerTask.cpp

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
#include "NTimerTask.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
EventLoopTimerUPP			NTimerTask::mTimerCallback = NewEventLoopTimerUPP(    TimerCallback);
EventLoopIdleTimerUPP		NTimerTask::mIdleCallback  = NewEventLoopIdleTimerUPP(IdleCallback);





//============================================================================
//		NTimerTask::NTimerTask : Constructor.
//----------------------------------------------------------------------------
NTimerTask::NTimerTask(void)
{
}





//============================================================================
//		NTimerTask::~NTimerTask : Destructor.
//----------------------------------------------------------------------------
NTimerTask::~NTimerTask(void)
{


	// Clean up
	RemoveTimer();
}





//============================================================================
//		NTimerTask::HasTimer : Do we have a timer?
//----------------------------------------------------------------------------
bool NTimerTask::HasTimer(NTimerID theTimer) const
{	bool						hasTimer;
	NTimerIDMapConstIterator	theIter;



	// Check our state
	if (theTimer == kTimerAny)
		hasTimer = !mTimers.empty();
	else
		{
		theIter  = mTimers.find(theTimer);
		hasTimer = (theIter != mTimers.end());
		}
	
	return(hasTimer);
}





//============================================================================
//		NTimerTask::AddTimer : Add a timer.
//----------------------------------------------------------------------------
OSStatus NTimerTask::AddTimer(EventTime			timeDelay,
								EventTime		timeRepeat,
								NTimerID		theTimer,
								bool			idleTimer,
								EventLoopRef	eventLoop)
{	EventLoopTimerRef	timerRef;
	OSStatus			theErr;



	// Remove the timer
	RemoveTimer(theTimer);



	// Install the timer
	if (idleTimer)
		theErr = InstallEventLoopIdleTimer(eventLoop, timeDelay, timeRepeat, mIdleCallback,  this, &timerRef);
	else
		theErr = InstallEventLoopTimer(    eventLoop, timeDelay, timeRepeat, mTimerCallback, this, &timerRef);
	
	NN_ASSERT_NOERR(theErr);



	// Save the timer
	if (theErr == noErr)
		mTimers[theTimer] = timerRef;
	
	return(theErr);
}





//============================================================================
//		NTimerTask::RemoveTimer : Remove a timer.
//----------------------------------------------------------------------------
void NTimerTask::RemoveTimer(NTimerID theTimer)
{	NTimerIDMapIterator		theIter;
	OSStatus				theErr;



	// Remove all timers
	if (theTimer == kTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			{
			theErr = RemoveEventLoopTimer(theIter->second);
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
			theErr = RemoveEventLoopTimer(theIter->second);
			NN_ASSERT_NOERR(theErr);
			
			mTimers.erase(theIter);
			}
		}
}





//============================================================================
//		NTimerTask::SetNextFireTime : Set the next fire time.
//----------------------------------------------------------------------------
void NTimerTask::SetNextFireTime(EventTime theTime, NTimerID theTimer)
{	NTimerIDMapIterator		theIter;
	OSStatus				theErr;



	// Adjust all timers
	if (theTimer == kTimerAll)
		{
		for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
			{
			theErr = SetEventLoopTimerNextFireTime(theIter->second, theTime);
			NN_ASSERT_NOERR(theErr);
			}
		}
	
	
	
	// Adjust a single timer
	else
		{
		theIter = mTimers.find(theTimer);
		if (theIter != mTimers.end())
			{
			theErr = SetEventLoopTimerNextFireTime(theIter->second, theTime);
			NN_ASSERT_NOERR(theErr);
			}
		}
}





//============================================================================
//		NTimerTask::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NTimerTask::DoTimer(NTimerID /*theTimer*/, TimerMsg /*theMsg*/)
{


	// Handle the timer
	//
	// Sub-classes should call through to their parent for unknown timers, to
	// allow timers to be stacked together in a single object hierarchy.
	//
	// E.g., object A may derive from NTimerTask to perform some periodic work.
	//
	// If a sub-class of A, B, also requires a timer then it can add its own
	// timer and call through to A::DoTimer for unknown timers to allow both
	// A and B to receive their timers independently.
	/*
	switch (theTimer) {
		case kMyTimer:
			DoMyPeriodicTask();
			break;
			
		default:
			CParentClass::DoTimer(theTimer, theMsg);
			break;
		}
	*/
}





//============================================================================
//		NTimerTask::InvokeTimer : Invoke a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NTimerTask::InvokeTimer(EventLoopTimerRef theTimer, TimerMsg theMsg)
{	NTimerIDMapConstIterator	theIter;



	// Invoke the timer
	for (theIter = mTimers.begin(); theIter != mTimers.end(); theIter++)
		{
		if (theIter->second == theTimer)
			{
			DoTimer(theIter->first, theMsg);
			return;
			}
		}
	
	NN_LOG("Unknown timer: 0x%8X (%d)", theTimer, theMsg);
}





//============================================================================
//		NTimerTask::TimerCallback : Timer callback.
//----------------------------------------------------------------------------
void NTimerTask::TimerCallback(EventLoopTimerRef theTimer, void *userData)
{	NTimerTask	*thisPtr = (NTimerTask *) userData;



	// Invoke the timer
	thisPtr->InvokeTimer(theTimer, kMsgTimerFire);
}





//============================================================================
//		NTimerTask::IdleCallback : Idle timer callback.
//----------------------------------------------------------------------------
void NTimerTask::IdleCallback(EventLoopTimerRef theTimer, EventLoopIdleTimerMessage theMsg, void *userData)
{	NTimerTask	*thisPtr = (NTimerTask *) userData;



	// Invoke the timer
	thisPtr->InvokeTimer(theTimer, (TimerMsg) theMsg);
}

