/*	NAME:
		NThreadUtilities.cpp

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetThread.h"
#include "NThreadUtilities.h"





//============================================================================
//		NThreadUtilities::IsMainThread : Is this the main thread?
//----------------------------------------------------------------------------
bool NThreadUtilities::IsMainThread(void)
{


	// Is this the main thread
	return(NTargetThread::IsMainThread());
}





//============================================================================
//		NThreadUtilities::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NThreadUtilities::Sleep(NTime theTime)
{


	// Sleep the thread
	NTargetThread::Sleep(theTime);
}





//============================================================================
//		NThreadUtilities::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NThreadUtilities::GetCPUCount(void)
{


	// Get the CPU count
	return(NTargetThread::GetCPUCount());
}





//============================================================================
//		NThreadUtilities::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NThreadUtilities::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Compare and swap
	return(NTargetThread::AtomicCompareAndSwap32(oldValue, newValue, theValue));
}





//============================================================================
//		NThreadUtilities::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	NTargetThread::AtomicAdd32(theDelta, theValue);
}





//============================================================================
//		NThreadUtilities::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicAnd32(theMask, theValue);
}





//============================================================================
//		NThreadUtilities::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicXor32(theMask, theValue);
}





//============================================================================
//		NThreadUtilities::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	NTargetThread::AtomicOr32(theMask, theValue);
}


