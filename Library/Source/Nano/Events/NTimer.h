/*	NAME:
		NTimer.h

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMER_HDR
#define NTIMER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"
#include "NFunctor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Timer state
typedef enum {
	kNTimerFired
} NTimerState;


// Timer IDs
typedef void *NTimerID;

static const NTimerID kTimerNone									= (NTimerID) NULL;
static const NTimerID kTimerAll										= (NTimerID) NULL;
static const NTimerID kTimerAny										= (NTimerID) NULL;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Timers
typedef nfunctor<void (NTimerState theState)>						NTimerFunctor;


// Maps
typedef std::map<NTimerID, NTimerFunctor>							NTimerMap;
typedef NTimerMap::iterator											NTimerMapIterator;
typedef NTimerMap::const_iterator									NTimerMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTimer {
public:
										 NTimer(void);
	virtual								~NTimer(void);


	// Is a timer installed?
	bool								HasTimer(NTimerID theTimer=kTimerAny) const;


	// Add/remove a timer
	NTimerID							AddTimer(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery=kNTimeNone);
	void								RemoveTimer(NTimerID theTimer=kTimerAll);


	// Reset a timer
	//
	// Defers the next fire time of the timer by the specified amount.
	void								ResetTimer(NTime fireAfter, NTimerID theTimer=kTimerAll);


private:
	mutable NMutexLock					mLock;
	NTimerMap							mTimers;
};





#endif // NTIMER_HDR


