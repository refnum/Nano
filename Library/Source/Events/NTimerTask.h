/*	NAME:
		NTimerTask.h

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMERTASK_HDR
#define NTIMERTASK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Timer messages
typedef enum {
	kMsgTimerFire									= 0,
	kMsgTimerIdleStart								= kEventLoopIdleTimerStarted,
	kMsgTimerIdleFire								= kEventLoopIdleTimerIdling,
	kMsgTimerIdleStop								= kEventLoopIdleTimerStopped
} TimerMsg;


// Timer IDs
typedef UInt32 NTimerID;

static const NTimerID kTimerDefault					= 0;
static const NTimerID kTimerAll						= (NTimerID) -1;
static const NTimerID kTimerAny						= (NTimerID) -1;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// maps
typedef std::map<NTimerID, EventLoopTimerRef>				NTimerIDMap;
typedef NTimerIDMap::iterator								NTimerIDMapIterator;
typedef NTimerIDMap::const_iterator							NTimerIDMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimerTask {
public:
										NTimerTask(void);
	virtual								~NTimerTask(void);


	// Do we have a timer?
	bool								HasTimer(NTimerID theTimer=kTimerAny) const;


	// Add a timer
	//
	// An NTimerTask can register multiple timers, each of which fire independently.
	OSStatus							AddTimer(EventTime		timeDelay,
												 EventTime		timeRepeat = 0.0f,
												 NTimerID		theTimer   = kTimerDefault,
												 bool			idleTimer  = false,
												 EventLoopRef	eventLoop  = GetMainEventLoop());


	// Remove a timer
	//
	// If no timer is supplied, all timers installed by this object will be removed.
	void								RemoveTimer(NTimerID theTimer=kTimerAll);


	// Set the next fire time
	//
	// If no timer is supplied, all timers installed by this object will be adjusted.
	void								SetNextFireTime(EventTime theTime, NTimerID theTimer=kTimerAll);


protected:
	// Handle a timer
	//
	// Sub-classes must call through if they do not recognize the timer, to allow
	// multiple timers to be registered against a hierarchy derived from NTimerTask.
	virtual void						DoTimer(NTimerID theTimer, TimerMsg theMsg);


private:
	void								InvokeTimer(EventLoopTimerRef theTimer, TimerMsg theMsg);

	static void							TimerCallback(EventLoopTimerRef theTimer, void *userData);
	static void							IdleCallback( EventLoopTimerRef theTimer, EventLoopIdleTimerMessage theMsg, void *userData);


private:
	NTimerIDMap							mTimers;

	static EventLoopTimerUPP			mTimerCallback;
	static EventLoopIdleTimerUPP		mIdleCallback;
};





#endif // NTIMERTASK_HDR


