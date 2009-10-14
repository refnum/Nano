/*	NAME:
		NTimer.h

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMER_HDR
#define NTIMER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>

#include "NFunctor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Timer messages
typedef enum {
	kMsgTimerFire											= 0,
	kMsgTimerIdleStart										= kEventLoopIdleTimerStarted,
	kMsgTimerIdleFire										= kEventLoopIdleTimerIdling,
	kMsgTimerIdleStop										= kEventLoopIdleTimerStopped
} TimerMsg;


// Timer IDs
typedef EventLoopTimerRef NTimerID;

static const NTimerID kTimerNone							= (NTimerID) NULL;
static const NTimerID kTimerAll								= (NTimerID) NULL;
static const NTimerID kTimerAny								= (NTimerID) NULL;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Timers
typedef nfunctor<void (TimerMsg theMsg)>					NTimerFunctor;


// Maps
typedef std::map<EventLoopTimerRef, NTimerFunctor>			NTimerMap;
typedef NTimerMap::iterator									NTimerMapIterator;
typedef NTimerMap::const_iterator							NTimerMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimer {
public:
										NTimer(void);
	virtual								~NTimer(void);


	// Do we have a timer?
	bool								HasTimer(NTimerID theTimer=kTimerAny) const;


	// Add a timer
	NTimerID							AddTimer(NTimerFunctor	theTimer,
												 EventTime		timeDelay,
												 EventTime		timeRepeat = 0.0f,
												 bool			idleTimer  = false,
												 EventLoopRef	eventLoop  = GetMainEventLoop());


	// Remove a timer
	void								RemoveTimer(NTimerID theTimer=kTimerAll);


	// Set the next fire time
	void								SetNextFireTime(EventTime theTime, NTimerID theTimer=kTimerAll);


private:
	void								InvokeTimer(EventLoopTimerRef theTimer, TimerMsg theMsg);

	static void							TimerCallback(EventLoopTimerRef theTimer, void *userData);
	static void							IdleCallback( EventLoopTimerRef theTimer, EventLoopIdleTimerMessage theMsg, void *userData);


private:
	NTimerMap							mTimers;

	static EventLoopTimerUPP			mTimerCallback;
	static EventLoopIdleTimerUPP		mIdleCallback;
};





#endif // NTIMER_HDR


