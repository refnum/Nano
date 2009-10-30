/*	NAME:
		NThreadUtilities.cpp

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <unistd.h>

#include "NThread.h"
#include "NSystemUtilities.h"
#include "NThreadUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const MPTaskID kMainThreadID								= MPCurrentTaskID();





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NFunctorThread : public NThread {
public:
										NFunctorThread(const NFunctor &theFunctor);
	virtual								~NFunctorThread(void);


protected:
	// Run the thread
	void								Run(void);


private:
	NFunctor							mFunctor;
};





//============================================================================
//		NFunctorThread::NFunctorThread : Constructor.
//----------------------------------------------------------------------------
NFunctorThread::NFunctorThread(const NFunctor &theFunctor)
{


	// Initialize ourselves
	mFunctor = theFunctor;

	SetAutoDelete(true);
}





//============================================================================
//		NFunctorThread::~NFunctorThread : Destructor.
//----------------------------------------------------------------------------
NFunctorThread::~NFunctorThread(void)
{
}





//============================================================================
//		NFunctorThread::Run : Run the thread.
//----------------------------------------------------------------------------
void NFunctorThread::Run(void)
{


	// Execute the thread
	mFunctor();
}





//============================================================================
//		NThreadUtilities::IsMainThread : Is this the main thread?
//----------------------------------------------------------------------------
#pragma mark -
bool NThreadUtilities::IsMainThread(void)
{


	// Check our state
	//
	// This assumes that the main thread performs static initialization.
	return(MPCurrentTaskID() == kMainThreadID);
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





//============================================================================
//		NThreadUtilities::DetachFunctor : Execute a functor on a new thread.
//----------------------------------------------------------------------------
void NThreadUtilities::DetachFunctor(const NFunctor &theFunctor)
{	NFunctorThread		*theThread;



	// Spawn the thread
	theThread = new NFunctorThread(theFunctor);
	if (theThread != NULL)
		theThread->Start();
}



