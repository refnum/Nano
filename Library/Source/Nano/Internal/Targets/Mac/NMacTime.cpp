/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void TimerCallback(CFRunLoopTimerRef cfTimer, void *userData)
{	TimerInfo			*timerInfo = (TimerInfo *) userData;



	// Validate our parameters
	NN_ASSERT(cfTimer == timerInfo->cfTimer);
	
	NN_UNUSED(cfTimer);



	// Invoke the timer
	timerInfo->theFunctor(kNTimerFired);
}





//============================================================================
//		NTargetTime::GetTime : Get the time.
//----------------------------------------------------------------------------
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

	fireTime          = CFAbsoluteTimeGetCurrent() + fireAfter;
	timerInfo         = new TimerInfo;
	timerContext.info = timerInfo;



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



	// Destroy the timer
	CFRunLoopRemoveTimer(CFRunLoopGetMain(), timerInfo->cfTimer, kCFRunLoopCommonModes);
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





