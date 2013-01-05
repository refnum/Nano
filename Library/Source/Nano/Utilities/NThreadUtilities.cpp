/*	NAME:
		NThreadUtilities.cpp

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetThread.h"
#include "NMathUtilities.h"
#include "NThreadUtilities.h"





//============================================================================
//		NThreadUtilities::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NThreadUtilities::GetCPUCount(void)
{


	// Get the CPU count
	return(NTargetThread::GetCPUCount());
}





//============================================================================
//		NThreadUtilities::DetachFunctor : Detach a functor.
//----------------------------------------------------------------------------
void NThreadUtilities::DetachFunctor(const NFunctor &theFunctor)
{


	// Detach the functor
	NTargetThread::ThreadCreate(theFunctor);
}





//============================================================================
//		NThreadUtilities::DelayFunctor : Delay a functor.
//----------------------------------------------------------------------------
void NThreadUtilities::DelayFunctor(const NFunctor &theFunctor, NTime theDelay, bool onMainThread)
{	NTimer		*theTimer;



	// Invoke immediately
	//
	// If we're to invoke on a new thread, or are already on the main
	// thread, we can invoke the functor directly without any delay.
	//
	// If we can't (we have a delay or we're not the main thread but
	// the functor must execute on the main thread), we fall through
	// to the timer case.
	if (NMathUtilities::IsZero(theDelay))
		{
		if (!onMainThread)
			{
			DetachFunctor(theFunctor);
			return;
			}
		
		else if (NThread::IsMain())
			{
			theFunctor();
			return;
			}
		}



	// Invoke with a delay
	theTimer = new NTimer;
	if (theTimer != NULL)
		theTimer->AddTimer(BindFunction(NThreadUtilities::DelayedFunctor, theTimer, theFunctor, onMainThread), theDelay);
}





//============================================================================
//		NThreadUtilities::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NThreadUtilities::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Compare and swap
	return(NTargetThread::AtomicCompareAndSwap32(theValue, oldValue, newValue));
}





//============================================================================
//		NThreadUtilities::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
SInt32 NThreadUtilities::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	return(NTargetThread::AtomicAdd32(theValue, theDelta));
}





//============================================================================
//		NThreadUtilities::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicAnd32(theValue, theMask);
}





//============================================================================
//		NThreadUtilities::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicXor32(theValue, theMask);
}





//============================================================================
//		NThreadUtilities::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicOr32(theValue, theMask);
}





//============================================================================
//		NThreadUtilities::DelayedFunctor : Execute a delayed functor.
//----------------------------------------------------------------------------
#pragma mark -
void NThreadUtilities::DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread)
{


	// Validate our state
	NN_ASSERT(NThread::IsMain());



	// Invoke the functor
	if (onMainThread)
		theFunctor();
	else
		NTargetThread::ThreadCreate(theFunctor);



	// Clean up
	delete theTimer;
}
