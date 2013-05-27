/*	NAME:
		NTimer.h

	DESCRIPTION:
		Timer object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTIMER_HDR
#define NTIMER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NMutex.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Timer state
typedef enum {
	kNTimerFired
} NTimerState;


// Timer IDs
typedef UIntPtr NTimerID;

static const NTimerID kNTimerNone									= (NTimerID) NULL;
static const NTimerID kNTimerAll									= (NTimerID) NULL;
static const NTimerID kNTimerAny									= (NTimerID) NULL;





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
	virtual							   ~NTimer(void);


	// Is a timer installed?
	bool								HasTimer(NTimerID theTimer=kNTimerAny) const;


	// Add/remove a timer
	NTimerID							AddTimer(const NTimerFunctor &theFunctor, NTime fireAfter, NTime fireEvery=kNTimeNone);
	void								RemoveTimer(NTimerID theTimer=kNTimerAll);


	// Reset a timer
	//
	// Defers the next fire time of the timer by the specified amount.
	void								ResetTimer(NTime fireAfter, NTimerID theTimer=kNTimerAll);


private:
	mutable NMutex						mLock;
	NTimerMap							mTimers;
};





#endif // NTIMER_HDR


