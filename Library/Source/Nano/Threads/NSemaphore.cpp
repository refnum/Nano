/*	NAME:
		NSemaphore.cpp

	DESCRIPTION:
		Thread semaphore.
	
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
#include "NSemaphore.h"





//============================================================================
//		NSemaphore::NSemaphore : Constructor.
//----------------------------------------------------------------------------
NSemaphore::NSemaphore(NIndex theValue)
{


	// Initialise ourselves
	mSemaphore = NTargetThread::SemaphoreCreate(theValue);
}





//============================================================================
//		NSemaphore::~NSemaphore : Destructor.
//----------------------------------------------------------------------------
NSemaphore::~NSemaphore(void)
{


	// Clean up
	NTargetThread::SemaphoreDestroy(mSemaphore);
}





//============================================================================
//		NSemaphore::Signal : SIgnal the semaphore.
//----------------------------------------------------------------------------
void NSemaphore::Signal(void)
{


	// Signal the semaphore
	NTargetThread::SemaphoreSignal(mSemaphore);
}





//============================================================================
//		NSemaphore::Wait : Wait for the semaphore.
//----------------------------------------------------------------------------
bool NSemaphore::Wait(NTime waitFor)
{	NStatus		theErr;



	// Wait for the semaphore
	theErr = NTargetThread::SemaphoreWait(mSemaphore, waitFor);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrTimeout);
	
	return(theErr == kNoErr);
}






