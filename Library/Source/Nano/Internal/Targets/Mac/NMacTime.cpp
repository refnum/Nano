/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/sysctl.h>

#include "NCFObject.h"
#include "NTargetTime.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	CFRunLoopTimerRef		cfTimer;
	NTimerFunctor			theFunctor;
} TimerInfo;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetTimerLock : Get the timer lock.
//----------------------------------------------------------------------------
static NMutexLock &GetTimerLock(void)
{	static NMutexLock		sLock;


	// Get the lock
	return(sLock);
}





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void TimerCallback(CFRunLoopTimerRef cfTimer, void */*userData*/)
{	StLock						acquireLock(GetTimerLock());
	CFRunLoopTimerContext		theContext;
	TimerInfo					*timerInfo;



	// Get the state we need
	//
	// Timers are always executed on the main thread, however they may be created
	// and destroyed on a different thread.
	//
	// This leads to two race conditions between TimerCallback/TimerDestroy.
	//
	//
	// Case 1:
	//
	//		Thread A:	Copy context.info as userData param for TimerCallback
	//		Thread B:	TimerDestroy()
	//		Thread A:	TimerCallback()
	//
	// Thread A copies the context.info value for use as the userData parameter
	// for TimerCallback, however Thread B can return from TimerDestroy before
	// TimerCallback is entered with the now-stale userData parameter.
	//
	// Fetching the current timerInfo state from the context allows us to sync
	// this value between the two threads, obtaining a valid pointer or NULL.
	//
	// Note that a shared TimerCallback/TimerDestroy lock does not help in this
	// case, as TimerDestroy may have completed between thread A starting to
	// call TimerCallback and TimerCallback being entered.
	//
	//
	// Case 2:
	//
	//		Thread A:	TimerCallback fetches timerInfo from context
	//		Thread B:	TimerDestroy()
	//		Thread A:	Invoke now-stale timerInfo pointer
	//
	// Although thread A obtains an atomic timerInfo/NULL pointer from the context,
	// thread B can destroy that timerInfo before thread A has a chance to use it.
	//
	// As such we need our own lock to ensure that the timer functor can never be
	// destroyed while we're about to execute it.
	CFRunLoopTimerGetContext(cfTimer, &theContext);
	
	timerInfo = (TimerInfo *) theContext.info;
	if (timerInfo != NULL)
		NN_ASSERT(timerInfo->cfTimer == cfTimer);



	// Invoke the timer
	if (timerInfo != NULL)
		timerInfo->theFunctor(kNTimerFired);
}





//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
#pragma mark -
NTime NTargetTime::GetTime(void)
{


	// Get the time
	return(CFAbsoluteTimeGetCurrent());
}





//============================================================================
//		NTargetTime::GetUpTime : Get the time since boot.
//----------------------------------------------------------------------------
NTime NTargetTime::GetUpTime(void)
{	int					mibNames[2] = { CTL_KERN, KERN_BOOTTIME }; 
	struct timeval		timeBoot, timeNow, theDelta;
	size_t				theSize;
	NTime				theTime;
	int					sysErr;



	// Get the state we need
	theSize = sizeof(timeBoot);
	sysErr  = sysctl(mibNames, 2, &timeBoot, &theSize, NULL, 0);
	NN_ASSERT_NOERR(sysErr);

	sysErr |= gettimeofday(&timeNow, NULL);
	NN_ASSERT_NOERR(sysErr);



	// Get the time since boot
	theTime = 0.0;

	if (sysErr == 0)
		{
		timersub(&timeNow, &timeBoot, &theDelta);
		theTime = ((NTime) theDelta.tv_sec) + (((NTime) theDelta.tv_usec) / 1000000.0);
		}
	
	return(theTime);
}





//============================================================================
//		NTargetTime::TimerCreate : Create a timer.
//----------------------------------------------------------------------------
NTimerID NTargetTime::TimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery)
{	CFRunLoopTimerContext		timerContext;
	TimerInfo					*timerInfo;
	CFAbsoluteTime				fireTime;



	// Get the state we need
	memset(&timerContext, 0x00, sizeof(timerContext));

	timerInfo         = new TimerInfo;
	timerContext.info = timerInfo;

	if (fireAfter <= 0.0)
		fireTime = CFRunLoopGetNextTimerFireDate(CFRunLoopGetMain(), kCFRunLoopCommonModes);
	else
		fireTime = CFAbsoluteTimeGetCurrent() + fireAfter;



	// Create the timer
	timerInfo->cfTimer    = CFRunLoopTimerCreate(kCFAllocatorNano, fireTime, fireEvery, 0, 0, TimerCallback, &timerContext);
	timerInfo->theFunctor = theFunctor;

	CFRunLoopAddTimer(CFRunLoopGetMain(), timerInfo->cfTimer, kCFRunLoopCommonModes);

	return((NTimerID) timerInfo);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{	TimerInfo		*timerInfo = (TimerInfo *) theTimer;
	StLock			acquireLock(GetTimerLock());



	// Destroy the timer
	//
	// We must acquire the timer lock to synchronise with TimerCallback().
	CFRunLoopTimerInvalidate(timerInfo->cfTimer);
	CFRelease(timerInfo->cfTimer);

	delete timerInfo;
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{	TimerInfo			*timerInfo = (TimerInfo *) theTimer;
	CFAbsoluteTime		fireTime;



	// Reset the timer
	fireTime = CFAbsoluteTimeGetCurrent() + fireAfter;
	
	CFRunLoopTimerSetNextFireDate(timerInfo->cfTimer, fireTime);
}





