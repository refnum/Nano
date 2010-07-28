/*	NAME:
		NThreadUtilities.h

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
	static UInt32						GetCPUCount(void);


	// Delay a functor
	//
	// The functor can be invoked on the main thread, or on a new one-shot thread.
	static void							DelayFunctor(const NFunctor &theFunctor, NTime theDelay=kNFunctorDelayTime, bool onMainThread=true);


	// Atomic operations
	static bool							AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue);
	static void							AtomicAdd32(           SInt32 &theValue, SInt32 theDelta);

	static void							AtomicAnd32(UInt32 &theValue, UInt32 theMask);
	static void							AtomicOr32( UInt32 &theValue, UInt32 theMask);
	static void							AtomicXor32(UInt32 &theValue, UInt32 theMask);


private:
	static void							DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread);
};





#endif // NTHREADUTILITIES_HDR


