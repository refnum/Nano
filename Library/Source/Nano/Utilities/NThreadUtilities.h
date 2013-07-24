/*	NAME:
		NThreadUtilities.h

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADUTILITIES_HDR
#define NTHREADUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NTime kNFunctorDelayTime								= 250 * kNTimeMillisecond;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadUtilities {
public:
	// Get the number of CPUs
	static NIndex						GetCPUCount(void);


	// Detach a functor
	//
	// Invokes the functor on a new thread.
	static void							DetachFunctor(const NFunctor &theFunctor);


	// Delay a functor
	//
	// The functor can be invoked on the main thread, or on a new one-shot thread.
	static void							DelayFunctor(const NFunctor &theFunctor, NTime theDelay=kNFunctorDelayTime, bool onMainThread=true);


	// Atomic compare and swap
	//
	// Inserts a read/write memory barrier.
	//
	// Returns as theValue was updated from oldValue to newValue.
	static bool							AtomicCompareAndSwap32 (int32_t &theValue, int32_t oldValue, int32_t newValue);
	static bool							AtomicCompareAndSwapPtr(void  *&theValue, void  *oldValue, void  *newValue);


	// Atomic addition
	//
	// Inserts a read/write memory barrier.
	//
	// Returns the new value stored in theValue after the addition.
	static int32_t						AtomicAdd32(int32_t &theValue, int32_t theDelta);


	// Atomic bit operations
	//
	// Inserts a read/write memory barrier.
	static void							AtomicAnd32(uint32_t &theValue, uint32_t theMask);
	static void							AtomicOr32( uint32_t &theValue, uint32_t theMask);
	static void							AtomicXor32(uint32_t &theValue, uint32_t theMask);


	// Insert a memory barrier
	//
	// Inserts a read/write memory barrier.
	static void							MemoryBarrier(void);


private:
	static void							DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread);
};





#endif // NTHREADUTILITIES_HDR


