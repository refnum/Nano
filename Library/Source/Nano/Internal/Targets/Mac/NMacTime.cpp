/*	NAME:
		NMacTime.cpp

	DESCRIPTION:
		Mac time support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFString.h"
#include "NMacTarget.h"
#include "NTargetThread.h"
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
//		GetTimeZone : Get a time zone.
//----------------------------------------------------------------------------
static NCFObject GetTimeZone(const NCFString &timeZone)
{	NCFObject		cfTimeZone;



	// Get the time zone
	if (timeZone == kNTimeZoneDefault)
		cfTimeZone.SetObject(CFTimeZoneCopyDefault());

	else
		cfTimeZone.SetObject(CFTimeZoneCreateWithName(kCFAllocatorNano, timeZone.GetObject(), true));

	NN_ASSERT(cfTimeZone.IsValid());

	return(cfTimeZone);
}





//============================================================================
//		TimerCallback : Timer callback.
//----------------------------------------------------------------------------
static void TimerCallback(CFRunLoopTimerRef cfTimer, void */*userData*/)
{	CFRunLoopTimerContext		theContext;
	TimerInfo					*timerInfo;



	// Get the state we need
	CFRunLoopTimerGetContext(cfTimer, &theContext);
	
	timerInfo = (TimerInfo *) theContext.info;
	if (timerInfo != NULL)
		NN_ASSERT(timerInfo->cfTimer == cfTimer);



	// Invoke the timer
	if (timerInfo != NULL)
		timerInfo->theFunctor(kNTimerFired);
}





//============================================================================
//		DoTimerCreate : Create a timer.
//----------------------------------------------------------------------------
static void DoTimerCreate(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery, NTimerID *theTimer)
{	CFRunLoopTimerContext		timerContext;
	TimerInfo					*timerInfo;
	CFAbsoluteTime				fireTime;



	// Get the state we need
	if (fireAfter <= 0.0)
		fireTime = CFRunLoopGetNextTimerFireDate(CFRunLoopGetMain(), kCFRunLoopCommonModes);
	else
		fireTime = CFAbsoluteTimeGetCurrent() + fireAfter;



	// Allocate the timer info
	timerInfo = new TimerInfo;
	*theTimer = (timerInfo == NULL) ? kNTimerNone : (NTimerID) timerInfo;

	if (timerInfo == NULL)
		return;

	memset(&timerContext, 0x00, sizeof(timerContext));

	timerContext.info = timerInfo;



	// Create the timer
	timerInfo->cfTimer    = CFRunLoopTimerCreate(kCFAllocatorNano, fireTime, fireEvery, 0, 0, TimerCallback, &timerContext);
	timerInfo->theFunctor = theFunctor;

	CFRunLoopAddTimer(CFRunLoopGetMain(), timerInfo->cfTimer, kCFRunLoopCommonModes);
}





//============================================================================
//		DoTimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
static void DoTimerDestroy(NTimerID theTimer)
{	TimerInfo		*timerInfo = (TimerInfo *) theTimer;



	// Destroy the timer
	CFRunLoopTimerInvalidate(timerInfo->cfTimer);
	CFRelease(timerInfo->cfTimer);

	delete timerInfo;
}





//============================================================================
//		DoTimerReset : Reset a timer.
//----------------------------------------------------------------------------
static void DoTimerReset(NTimerID theTimer, NTime fireAfter)
{	TimerInfo			*timerInfo = (TimerInfo *) theTimer;
	CFAbsoluteTime		fireTime;



	// Reset the timer
	fireTime = CFAbsoluteTimeGetCurrent() + fireAfter;
	
	CFRunLoopTimerSetNextFireDate(timerInfo->cfTimer, fireTime);
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
{	struct timeval		timeBoot, timeNow, theDelta;
	NTime				theTime;
	int					sysErr;



	// Get the state we need
	timeBoot = NMacTarget::GetSysctl<struct timeval>(CTL_KERN, KERN_BOOTTIME);
	sysErr   = gettimeofday(&timeNow, NULL);
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
{	NTimerID	theTimer;



	// Create the timer
	//
	// Nano allows timers to be created/manipulated on any thread, however they
	// always execute on the main thread.
	//
	// Although this could be achieved by creating CFTimers on the main run loop,
	// it would require synchronisation between the timer manipulation calls and
	// the timer execution.
	//
	// This would then require a shared lock (thread A is executing a timer while
	// thread B tries to destroy it), and can be difficult to synchronise without
	// deadlock (thread A is executing a timer which requests that thread B destroy
	// a different timer, thread B now blocks waiting for thread A).
	//
	// Instead we always execute and manipulate timers via the main thread.
	theTimer = kNTimerNone;
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerCreate, theFunctor, fireAfter, fireEvery, &theTimer));

	return(theTimer);
}





//============================================================================
//		NTargetTime::TimerDestroy : Destroy a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerDestroy(NTimerID theTimer)
{


	// Destroy the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerDestroy, theTimer));
}





//============================================================================
//		NTargetTime::TimerReset : Reset a timer.
//----------------------------------------------------------------------------
void NTargetTime::TimerReset(NTimerID theTimer, NTime fireAfter)
{


	// Reset the timer
	NTargetThread::ThreadInvokeMain(BindFunction(DoTimerReset, theTimer, fireAfter));
}





//============================================================================
//		NTargetTime::ConvertTimeToDate : Convert a UTC time to a date.
//----------------------------------------------------------------------------
NGregorianDate NTargetTime::ConvertTimeToDate(NTime theTime, const NString &timeZone)
{	NCFObject			cfTimeZone;
	NGregorianDate		theDate;
	CFGregorianDate		cfDate;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	cfTimeZone = GetTimeZone(timeZone);
	cfTime     = theTime - kNEpochTimeSince2001;



	// Convert the time
	cfDate = CFAbsoluteTimeGetGregorianDate(cfTime, cfTimeZone);
	
	theDate.year     = cfDate.year;
	theDate.month    = cfDate.month;
	theDate.day      = cfDate.day;
	theDate.hour     = cfDate.hour;
	theDate.minute   = cfDate.minute;
	theDate.second   = cfDate.second;
	theDate.timeZone = NCFString(CFTimeZoneGetName(cfTimeZone), false);
	
	return(theDate);
}





//============================================================================
//		NTargetTime::ConvertDateToTime : Convert a date to a UTC time.
//----------------------------------------------------------------------------
NTime NTargetTime::ConvertDateToTime(const NGregorianDate &theDate)
{	NCFObject			cfTimeZone;
	NTime				theTime;
	CFGregorianDate		cfDate;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	cfTimeZone = GetTimeZone(theDate.timeZone);

	cfDate.year   = theDate.year;
	cfDate.month  = theDate.month;
	cfDate.day    = theDate.day;
	cfDate.hour   = theDate.hour;
	cfDate.minute = theDate.minute;
	cfDate.second = theDate.second;



	// Convert the date
	cfTime  = CFGregorianDateGetAbsoluteTime(cfDate, cfTimeZone);
	theTime = cfTime + kNEpochTimeSince2001;
	
	return(theTime);
}





//============================================================================
//		NTargetTime::GetDayOfWeek : Get the day of the week.
//----------------------------------------------------------------------------
NIndex NTargetTime::GetDayOfWeek(const NGregorianDate &theDate)
{	NCFObject			cfTimeZone;
	NIndex				theIndex;
	NTime				theTime;
	CFAbsoluteTime		cfTime;



	// Get the state we need
	theTime    = ConvertDateToTime(theDate);
	cfTimeZone = GetTimeZone(theDate.timeZone);
	cfTime     = theTime - kNEpochTimeSince2001;



	// Get the day of the week
	theIndex = CFAbsoluteTimeGetDayOfWeek(cfTime, cfTimeZone);
	NN_ASSERT(theIndex >= 1 && theIndex <= 7);
	
	return(theIndex);
}





