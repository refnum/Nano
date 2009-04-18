/*	NAME:
		NMacThread.cpp

	DESCRIPTION:
		Mac thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <unistd.h>

#include <libKern/OSAtomic.h>

#include "NTargetThread.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const pthread_t kMainThreadID								= pthread_self();





//============================================================================
//		NTargetThread::IsMainThread : Is this the main thread?
//----------------------------------------------------------------------------
bool NTargetThread::IsMainThread(void)
{


	// Check our state
	//
	// This assumes that the main thread performs static initialization.
	return(pthread_self() == kMainThreadID);
}





//============================================================================
//		NTargetThread::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::Sleep(NTime theTime)
{


	// Sleep the thread
	usleep((useconds_t) (theTime / kNTimeMicrosecond));
}





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NTargetThread::GetCPUCount(void)
{


	// Get the CPU count
	return(MPProcessorsScheduled());
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Compare and swap
	return(OSAtomicCompareAndSwap32Barrier(oldValue, newValue, (int32_t *) &theValue));
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	OSAtomicAdd32Barrier(theDelta, (int32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicAnd32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicXor32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicOr32Barrier(theMask, (uint32_t *) &theValue);
}

