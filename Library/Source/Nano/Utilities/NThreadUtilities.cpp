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
void NThreadUtilities::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	NTargetThread::AtomicAdd32(theValue, theDelta);
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


