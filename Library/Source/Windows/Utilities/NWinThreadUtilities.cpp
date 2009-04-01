/*	NAME:
		NWinThreadUtilities.cpp

	DESCRIPTION:
		Windows thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"





//============================================================================
//		NThreadUtilities::IsMainThread : Is this the main thread?
//----------------------------------------------------------------------------
bool NThreadUtilities::IsMainThread(void)
{
	// dair, to do
	return(true);
}





//============================================================================
//		NThreadUtilities::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NThreadUtilities::Sleep(NTime theTime)
{
	// dair, to do
}





//============================================================================
//		NThreadUtilities::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NThreadUtilities::GetCPUCount(void)
{


	// dair, to do
	return(1);
}





//============================================================================
//		NThreadUtilities::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NThreadUtilities::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{
	// dair, to do
	theValue = newValue;
	return(true);
}





//============================================================================
//		NThreadUtilities::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// dair, to do
	theValue += theDelta;
}





//============================================================================
//		NThreadUtilities::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	theValue &= theMask;
}





//============================================================================
//		NThreadUtilities::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	theValue ^= theMask;
}





//============================================================================
//		NThreadUtilities::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NThreadUtilities::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	theValue |= theMask;
}
