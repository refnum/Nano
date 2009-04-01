/*	NAME:
		NMacThreadUtilities.cpp

	DESCRIPTION:
		Mac thread utilities.
	
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

#include "NThreadUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const MPTaskID kMainThreadID								= MPCurrentTaskID();





//============================================================================
//		NThreadUtilities::IsMainThread : Is this the main thread?
//----------------------------------------------------------------------------
bool NThreadUtilities::IsMainThread(void)
{


	// Check our state
	//
	// This assumes that the main thread performs static initialization.
	return(MPCurrentTaskID() == kMainThreadID);
}





//============================================================================
//		NThreadUtilities::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NThreadUtilities::Sleep(NTime theTime)
{


	// Sleep the thread
	usleep((useconds_t) (theTime / kNTimeMicrosecond));
}





//============================================================================
//		NThreadUtilities::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NThreadUtilities::GetCPUCount(void)
{


	// Get the CPU count
	return(MPProcessorsScheduled());
}





//============================================================================
//		NThreadUtilities::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NThreadUtilities::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Compare and swap
	return(CompareAndSwap(oldValue, newValue, &theValue));
}





//============================================================================
//		NThreadUtilities::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	AddAtomic(theDelta, &theValue);
}





//============================================================================
//		NThreadUtilities::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	BitAndAtomic(theMask, &theValue);
}





//============================================================================
//		NThreadUtilities::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	BitXorAtomic(theMask, &theValue);
}





//============================================================================
//		NThreadUtilities::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	BitOrAtomic(theMask, &theValue);
}
